#include <fbxsdk.h>
#include <vector>
#include <fstream>


//TODO: Change to a unified header for both importer and exporter.
struct Vector3
{
    Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Vertex
{
    Vertex(float x, float y, float z, float u, float v) :
        pos(x, y, z), u(u), v(v) {}

    Vector3 pos;
    float u = 0;
    float v = 0;
};

struct Data
{
    std::vector<Vertex> vertices;
    std::vector<int> indices;
};

std::vector<Data> loadFBX(std::string path);
void FBXgetScene(std::vector<Data>& data, FbxNode* root = nullptr);
void FBXgetMesh(FbxNode* node, std::vector<Data>& data);
void FBXgetLODGroup(FbxNode* node, std::vector<Data>& data);
FbxManager* manager = nullptr;
FbxScene* scene = nullptr;


int main()
{
    auto data = loadFBX("cube.fbx");



    return 0;
}


std::vector<Data> loadFBX(std::string path)
{
    std::vector<Data> data;
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

void FBXgetScene(std::vector<Data>& data, FbxNode* root)
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

void FBXgetMesh(FbxNode* node, std::vector<Data>& data)
{
    FbxMesh* mesh(node->GetMesh());

    if (mesh->RemoveBadPolygons() == -1)
        return;

    FbxGeometryConverter triangulator(manager);
    mesh = static_cast<FbxMesh*>(triangulator.Triangulate(mesh, true));

    if (mesh->RemoveBadPolygons() == -1)
        return;

    int vertexCount = mesh->GetControlPointsCount();
    FbxVector4* verticies = mesh->GetControlPoints();
    int indexCount = mesh->GetPolygonVertexCount();
    int* indices = mesh->GetPolygonVertices();

    Data l_data;

    std::vector<int> vertex_ref(vertexCount, -1);

    for (int i = 0; i < indexCount; i++)
    {
        int index = indices[i];

        if (vertex_ref[index] != -1) //If vertex encountered previously
        {
            l_data.indices.push_back(vertex_ref[index]);
        }

        else //new vertex
        {
            FbxVector4 vertex = verticies[index];
            l_data.indices.push_back(l_data.vertices.size());
            vertex_ref[index] = l_data.indices[i];
            l_data.vertices.push_back(Vertex(vertex[0], vertex[1], vertex[2], 0, 0));
        }
    }

    data.push_back(l_data);
}

void FBXgetLODGroup(FbxNode* node, std::vector<Data>& data)
{
}
