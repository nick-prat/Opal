//
// Created by nprat on 7/14/16.
//

#include <memory>
#include "assimpmodel.h"
#include "assimploader.h"


AssimpLoader::AssimpLoader()
{

}

AssimpLoader::~AssimpLoader()
{

}

AssimpModel* AssimpLoader::LoadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if(!scene)
    {
        return nullptr;
    }

    return nullptr;
}
