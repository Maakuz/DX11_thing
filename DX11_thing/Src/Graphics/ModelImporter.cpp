#include "ModelImporter.h"
#include "../StringSplitter.h"
#include <fstream>
#include <sstream>

ModelImporter::Data ModelImporter::loadObjModel(std::string path) //TODO: HANDLE o & s in obj files
{
    ModelImporter::Data data;

    std::ifstream file(path);

    if (file.is_open())
    {
        std::vector<Pos> posVec;
        std::vector<UV> uvVec;
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
                    Pos pos;
                    sstream >> pos.x;
                    sstream >> pos.y;
                    sstream >> pos.z;
                    posVec.push_back(pos);
                }

                if (word == "vt")
                {
                    UV uv;
                    sstream >> uv.u;
                    sstream >> uv.v;
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
                            Pos pos = posVec[iPos];
                            UV uv = uvVec[iuv];

                            data.vertices.push_back(Vertex(pos.x, pos.y, pos.z, 1 - uv.u, 1 - uv.v)); //Inverting UV coords

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

    return data;
}