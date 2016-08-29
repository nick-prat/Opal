#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <iostream>

#include "assimpmodel.h"
#include "assimploader.h"

std::shared_ptr<AssimpModel> AssimpLoader::LoadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if(!scene)
    {
        std::cout << "Couldn't open model: " << filename << std::endl;
        return nullptr;
    }

    std::cout << "Loading model: " << filename << std::endl;

    auto model = std::make_shared<AssimpModel>();

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        AssimpModel::AssimpMesh aMesh;

        std::vector<glm::vec3> vertices;
        for(int j = 0; j < mesh->mNumVertices; j++)
        {
            aiVector3D vertex = mesh->mVertices[j];
            vertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
        }
        aMesh.SetVertices(vertices);

        if(mesh->HasNormals())
        {
            std::vector<glm::vec3> normals;
            for(int j = 0; j < mesh->mNumVertices; j++)
            {
                aiVector3D normal = mesh->mNormals[j];
                normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
            }
            aMesh.SetNormals(normals);
        }

        std::vector<std::vector<glm::vec2>> texCoords;
        for(int j = 0; mesh->HasTextureCoords(j); j++)
        {
            std::vector<glm::vec2> texCoordsPart;
            for(int k = 0; k < mesh->mNumVertices; k++)
            {
                aiVector3D texCoord = mesh->mTextureCoords[j][k];
                texCoordsPart.push_back(glm::vec2(texCoord.x, texCoord.y));
            }
            texCoords.push_back(texCoordsPart);
        }
        if(texCoords.size() > 0)
        {
            aMesh.SetTexCoords(texCoords);
        }

        if(mesh->HasFaces())
        {
            std::vector<uint> indices;
            for(int j = 0; j < mesh->mNumFaces; j++)
            {
                aiFace face = mesh->mFaces[j];
                for(int k = 0; k < face.mNumIndices; k++)
                {
                    indices.push_back(face.mIndices[k]);
                }
            }
            aMesh.SetIndices(indices);
        }
        model->AddMesh(aMesh);
    }

    return model;
}
