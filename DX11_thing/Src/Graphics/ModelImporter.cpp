#include "ModelImporter.h"
#include "../StringSplitter.h"
#include <fstream>
#include <sstream>

std::vector<Mesh> ModelImporter::loadObjModel(std::string path, ID3D11Device* device, ID3D11DeviceContext* context)
{
    ModelImporter::MeshData data;

    std::ifstream file(path);

    if (file.is_open())
    {
        std::vector<DirectX::XMFLOAT3> posVec;
        std::vector<DirectX::XMFLOAT2> uvVec;
        std::vector<std::string> compositionName;

        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);

            if (line[0] != '#') // comment symbol
            {
                std::stringstream sstream(line);
                std::string word;
                sstream >> word;

                if (word == "v")
                {
                    DirectX::XMFLOAT3 pos;
                    sstream >> pos.x;
                    sstream >> pos.y;
                    sstream >> pos.z;
                    posVec.push_back(pos);
                }

                if (word == "vt")
                {
                    DirectX::XMFLOAT2 uv;
                    sstream >> uv.x;
                    sstream >> uv.y;
                    uvVec.push_back(uv);
                }

                if (word == "f")
                {
                    std::vector<std::string> faces;
                    while (!sstream.eof())
                    {
                        sstream >> word;
                        faces.push_back(word);
                    }

                    for (std::string& str : faces)
                    {
                        int found = -1;
                        for (int i = 0; i < compositionName.size() && found == -1; i++)
                        {
                            if (str.compare(compositionName[i]) == 0)
                                found = i;
                        }

                        if (found == -1)
                        {
                            auto split = StringSplitter::split(str, '/');
                            int iPos = std::stoi(split[0]) - 1;
                            int iuv = std::stoi(split[1]) - 1;
                            DirectX::XMFLOAT3 pos = posVec[iPos];
                            DirectX::XMFLOAT2 uv = uvVec[iuv];

                            data.vertices.push_back(Vertex(pos.x, pos.y, pos.z, 1 - uv.x, 1 - uv.y, 0, 0, 0)); //Inverting UV coords

                            compositionName.push_back(str);
                        }
                    }

                    int index[] = { 0, 1, 2, 2, 3, 0 };
                    int indexUsage = 3;

                    if (faces.size() == 4) //convert quads to triangles
                        indexUsage = 6;

                    for (int j = 0; j < indexUsage; j++)
                    {
                        for (int i = 0; i < compositionName.size(); i++)
                        {
                            if (faces[index[j]].compare(compositionName[i]) == 0)
                            {
                                data.indices.push_back(i);
                                break;
                            }
                        }
                    }
                }
            }
        }   
    }

    else
        LOG_ERROR(path + "\nFile not found");

    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    textures.push_back(Texture(device, Colors::UnloadedTextureColor, TextureType::Diffuse));
    meshes.push_back(Mesh(device, context, data.vertices, data.indices, textures));

    return meshes;
}

std::vector<Mesh> ModelImporter::loadModel(std::string path, ID3D11Device* device, ID3D11DeviceContext* context)
{
    std::vector<Mesh> meshes;

    std::ifstream infile(path, std::ios::binary);

    if (infile.is_open())
    {
        int meshCount = 0;
        infile.read((char*)&meshCount, sizeof(int));
        std::vector<ModelImporter::MeshData> rawData(meshCount);

        for (int i = 0; i < meshCount; i++)
        {
            int vertexCount = 0;
            infile.read((char*)&vertexCount, sizeof(int));
            rawData[i].vertices.resize(vertexCount);
            infile.read((char*)rawData[i].vertices.data(), sizeof(Vertex) * vertexCount);

            int indexCount = 0;
            infile.read((char*)&indexCount, sizeof(int));
            rawData[i].indices.resize(indexCount);
            infile.read((char*)rawData[i].indices.data(), sizeof(DWORD) * indexCount);

            int materialCount = 0;
            infile.read((char*)&materialCount, sizeof(int));
            rawData[i].materials.resize(materialCount);
            infile.read((char*)rawData[i].materials.data(), sizeof(Material) * materialCount);
        }

        infile.close();
        
        for (ModelImporter::MeshData& data : rawData)
        {
            std::vector<Texture> textures;
            textures.push_back(Texture(device, !data.materials.empty() ? data.materials[0].diffuse : Colors::UnloadedTextureColor, TextureType::Diffuse));

            meshes.push_back(Mesh(device, context, data.vertices, data.indices, textures));
        }
    }

    else
        ErrorLogger::log("File " + path + " cannot be opened. Is it in use or nonexistant?");

    return meshes;
}
