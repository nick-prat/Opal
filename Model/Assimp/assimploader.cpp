//
// Created by nprat on 7/14/16.
//

#include <memory>
#include "assimploader.h"


AssimpLoader::AssimpLoader()
{

}

AssimpLoader::~AssimpLoader()
{

}

bool AssimpLoader::LoadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), 0);

    if(!scene)
    {
        return false;
    }

    return false;
}

