#include <glm/glm.hpp>
#include <FreeImage.h>
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

    //aiMaterial** materials = scene->mMaterials;
    for(uint i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        std::vector<AssimpModel::Vertex> vertices;

        for(uint j = 0; j < mesh->mNumVertices; j++)
        {
            AssimpModel::Vertex vertex;

            vertex.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
            vertex.normal = (mesh->HasNormals()) ? glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z) : glm::vec3(0.0f, 0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        std::vector<uint> indices;
        if(mesh->HasFaces())
        {
            for(uint j = 0; j < mesh->mNumFaces; j++)
            {
                aiFace face = mesh->mFaces[j];
                for(uint k = 0; k < face.mNumIndices; k++)
                {
                    indices.push_back(face.mIndices[k]);
                }
            }
        }
        else
        {
            return nullptr;
        }

        model->AddMesh(AssimpModel::AssimpMesh(vertices, indices));
    }

    return model;
}
