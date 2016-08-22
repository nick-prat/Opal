//
// Created by nprat on 7/14/16.
//

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <iostream>

#include "assimpmodel.h"
#include "assimploader.h"


AssimpLoader::AssimpLoader()
{

}

AssimpLoader::~AssimpLoader()
{

}

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

    auto model = std::make_unique<AssimpModel>();

    return model;
}
