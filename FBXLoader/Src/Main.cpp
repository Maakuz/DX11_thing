#include <fbxsdk.h>
#include <vector>
#include <fstream>
#include "../../DX11_thing/Src/Graphics/Vertex.h"
#include "../../DX11_thing/Src/Graphics/Material.h"
#include "../../DX11_thing/Src/Graphics/ModelImporter.h"

using namespace DirectX;

//TODO: Multiple material mesh handling

std::vector<ModelImporter::MeshData> loadFBX(std::string path);
void FBXgetScene(std::vector<ModelImporter::MeshData>& data, FbxNode* root = nullptr);
void FBXgetMesh(FbxNode* node, std::vector<ModelImporter::MeshData>& data);
void FBXgetLODGroup(FbxNode* node, std::vector<ModelImporter::MeshData>& data);
FbxManager* manager = nullptr;
FbxScene* scene = nullptr;


int main()
{
    const char* fileName = "cube";
    auto meshes = loadFBX(std::string(fileName) + ".fbx");

    std::ofstream file(std::string(fileName) + ".bah", std::ios::binary);

    //Structure
    //mesh count            (int)
    //    vertex count      (int)
    //        vertices      (Vertex)
    //    index count       (int)
    //        indices       (int)


    if (file.is_open())
    {
        int i = meshes.size();
        file.write((char*)&i, sizeof(int));
        for (const auto& mesh : meshes)
        {
            int vertexCount = mesh.vertices.size();
            file.write((char*)&vertexCount, sizeof(int));
            file.write((char*)mesh.vertices.data(), sizeof(Vertex) * vertexCount);

            int indexCount = mesh.indices.size();
            file.write((char*)&indexCount, sizeof(int));
            file.write((char*)mesh.indices.data(), sizeof(DWORD) * indexCount);

            int materialCount = mesh.materials.size();
            file.write((char*)&materialCount, sizeof(int));
            file.write((char*)mesh.materials.data(), sizeof(Material) * materialCount);

        }

        file.close();
    }

    ////test
    std::ifstream infile("cube.bah", std::ios::binary);

    if (infile.is_open())
    {
        int meshCount = 0;
        infile.read((char *)&meshCount, sizeof(int));
        std::vector<ModelImporter::MeshData> indata(meshCount);

        for (int i = 0; i < meshCount; i++)
        {
            int vertexCount = 0;
            infile.read((char*)&vertexCount, sizeof(int));
            indata[i].vertices.resize(vertexCount);
            infile.read((char*)indata[i].vertices.data(), sizeof(Vertex) * vertexCount);

            int indexCount = 0;
            infile.read((char*)&indexCount, sizeof(int));
            indata[i].indices.resize(indexCount);
            infile.read((char*)indata[i].indices.data(), sizeof(DWORD) * indexCount);

            int materialCount = 0;
            infile.read((char*)&materialCount, sizeof(int));
            indata[i].materials.resize(materialCount);
            infile.read((char*)indata[i].materials.data(), sizeof(Material) * materialCount);
        }

        infile.close();
    }

    return 0;
}


std::vector<ModelImporter::MeshData> loadFBX(std::string path)
{
    std::vector<ModelImporter::MeshData> data;
    manager = FbxManager::Create();
    if (manager)
    {
        FbxIOSettings* ios(FbxIOSettings::Create(manager, IOSROOT));
        manager->SetIOSettings(ios);

        scene = (FbxScene::Create(manager, "Scene"));

        if (scene)
        {
            FbxImporter* importer(FbxImporter::Create(manager, "Importer"));
            if (importer)
            {
                importer->Initialize(path.c_str(), -1, manager->GetIOSettings());
                importer->Import(scene);
                importer->Destroy();
            }

            FBXgetScene(data);

            scene->Destroy();
        }
        manager->Destroy();
    }

    return data;
}

void FBXgetScene(std::vector<ModelImporter::MeshData>& data, FbxNode* root)
{
    if (!root)
    {
        root = scene->GetRootNode();
        if (!root) return;
    }

    for (int i = 0; i < root->GetChildCount(); i++)
    {
        FbxNode* node(root->GetChild(i));
        if (!node) continue;

        if (node->GetMesh())
        {
            FBXgetMesh(node, data);
        }

        else if (node->GetLodGroup())
        {
            FBXgetLODGroup(node, data);
        }

        else
            FBXgetScene(data, node);

    }
}

void FBXgetMesh(FbxNode* node, std::vector<ModelImporter::MeshData>& data)
{
    FbxMesh* mesh(node->GetMesh());

    if (mesh->RemoveBadPolygons() == -1)
        return;

    FbxGeometryConverter triangulator(manager);
    mesh = static_cast<FbxMesh*>(triangulator.Triangulate(mesh, true));

    if (mesh->RemoveBadPolygons() == -1)
        return;

    std::vector<XMFLOAT3> l_positions;
    std::vector<int> l_indices;
    std::vector<int> l_materialIndices;
    std::vector<int> l_materialUsed;
    std::vector<XMFLOAT3> l_normals;
    std::vector<XMFLOAT4> l_tangents;
    std::vector<std::vector<XMFLOAT2>> l_uvs;

    int vertexCount = mesh->GetControlPointsCount();
    FbxVector4* verticies = mesh->GetControlPoints();
    int indexCount = mesh->GetPolygonVertexCount();
    int* indices = mesh->GetPolygonVertices();

    //for (int i = 0; i < indexCount; i++)
    //{
    //    printf("%i\n", indices[i]);
    //}
    //
    //for (int i = 0; i < vertexCount; i++)
    //{
    //    printf("x:%f, y:%f, z:%f\n", verticies[i][0], verticies[i][1], verticies[i][2]);
    //}

    std::vector<int> vertex_ref(vertexCount, -1);


    for (int i = 0; i < indexCount; i++)
    {
        int index = indices[i];

        if (vertex_ref[index] != -1) //If vertex encountered previously
        {
            l_indices.push_back(vertex_ref[index]);
        }

        else //new vertex
        {
            FbxVector4 vertex = verticies[index];
            l_indices.push_back(l_positions.size());
            vertex_ref[index] = l_indices[i];
            l_positions.push_back(XMFLOAT3(-vertex[0], vertex[2], vertex[1])); //FLIPPED DUE TO Z NOT CHANGING TO Y DURING BLENDER EXPORT
            //printf("x:%f, y:%f, z:%f\n", vertex[0], vertex[1], vertex[2]);
        }
    }

    //Material
    FbxLayerElementArrayTemplate<int>* materialIndices;
    if (mesh->GetMaterialIndices(&materialIndices))
    {
        for (int i = 0; i < vertexCount; i++)
        {
            int index = materialIndices->GetAt(i);

            l_materialIndices.push_back(index);
            if (std::find(l_materialUsed.begin(), l_materialUsed.end(), index) == l_materialUsed.end())
                l_materialUsed.push_back(index);
        }
    }

    int materialCount = node->GetMaterialCount();
    Material l_material;
    for (int i = 0; i < materialCount; i++)
    {
        FbxSurfaceMaterial* material = node->GetMaterial(i);
        if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
        {
            auto ambient = ((FbxSurfacePhong*)material)->Ambient;
            auto diffuse = ((FbxSurfacePhong*)material)->Diffuse;
            auto specular = ((FbxSurfacePhong*)material)->Specular;
            //Emissive color, transparency, shininess, reflection can go in here as well

            l_material.ambient = Color(ambient.Get()[0] * 255, ambient.Get()[1] * 255, ambient.Get()[2] * 255);
            l_material.diffuse = Color(diffuse.Get()[0] * 255, diffuse.Get()[1] * 255, diffuse.Get()[2] * 255);
            l_material.specular = Color(specular.Get()[0] * 255, specular.Get()[1] * 255, specular.Get()[2] * 255);
        }
    }
    

    //Normals
    FbxArray<FbxVector4> normals;
    if (mesh->GenerateNormals() && mesh->GetPolygonVertexNormals(normals) && normals.Size() > 0)
    {
        int normalCount = normals.Size();
        for (int i = 0; i < normalCount; i++)
        {
            l_normals.push_back(XMFLOAT3(normals[i][0], normals[i][2], normals[i][1])); //FLIPPED DUE TO Z NOT CHANGING TO Y DURING BLENDER EXPORT
        }
    }

    //Tangent
    FbxLayerElementArrayTemplate<FbxVector4>* tangents(nullptr);

    if (mesh->GenerateTangentsData() && mesh->GetTangents(&tangents) && tangents->GetCount() > 0)
    {
        int tangentCount = tangents->GetCount();
        for (int i = 0; i < tangentCount; i++)
        {
            FbxVector4 t(tangents->GetAt(i));
            l_tangents.push_back(XMFLOAT4(t[0], t[1], t[2], t[3]));
        }
    }

    //Get UV
    FbxStringList uvNames;
    mesh->GetUVSetNames(uvNames);
    
    int uvSetCount = uvNames.GetCount();
    l_uvs.resize(uvSetCount);
    for (int i = 0; i < uvSetCount; i++)
    {
        FbxArray<FbxVector2> uvs;
        if (mesh->GetPolygonVertexUVs(uvNames.GetStringAt(i), uvs))
        {
            int uvCount = uvs.Size();
            for (int j = 0; j < uvCount; j++)
            {
                l_uvs[i].push_back(XMFLOAT2(uvs[j][0], uvs[j][1]));
            }
        }
    }

    //Constructing final vertices
    //Might have to do some vertex_ref magic to get the material indices to work
    ModelImporter::MeshData l_data;
    for (int i = 0; i < l_indices.size(); i++)
    {
        Vertex vertex(l_positions[l_indices[i]], l_normals[i], l_uvs[0][i]);

        auto it = std::find(l_data.vertices.begin(), l_data.vertices.end(), vertex);
        if (it == l_data.vertices.end()) //Not found
        {
            l_data.indices.push_back(l_data.vertices.size());
            l_data.vertices.push_back(vertex);
        }

        else
        {
            int index = it - l_data.vertices.begin();
            l_data.indices.push_back(index);
        }
    }

    l_data.materials.push_back(l_material);

    //for (int i : l_data.indices)
    //{
    //    printf("x:%f, y:%f, z:%f\n", l_positions[l_data.indices[i]].x, l_positions[l_data.indices[i]].y, l_positions[l_data.indices[i]].z);
    //}

    data.push_back(l_data);
}

void FBXgetLODGroup(FbxNode* node, std::vector<ModelImporter::MeshData>& data)
{
    //https://www.youtube.com/watch?v=NeWPCm5NsIw
    //Good video whenever it is time to implement 14:30 ish
}
