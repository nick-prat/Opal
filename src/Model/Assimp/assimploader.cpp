//
// Created by nprat on 7/14/16.
//

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <iostream>

#include "assimpmodel.h"
#include "assimploader.h"

std::unique_ptr<AssimpModel> AssimpLoader::LoadModel(std::string filename)
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

    auto model = std::make_unique<AssimpModel>();

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        
        aiMesh* mesh = scene->mMeshes[i];

        for(int j = 0; j < mesh->mNumFaces; j++)
        {
            const aiFace& face = mesh->mFaces[j];
            for(int k = 0; k < 3; k++)
            {

            }
        }
    }

    return model;
}
