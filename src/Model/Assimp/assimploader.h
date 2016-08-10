//
// Created by nprat on 7/14/16.
//

#ifndef OPENGL_ASSIMP_H
#define OPENGL_ASSIMP_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpLoader
{
public:
    AssimpLoader();
    ~AssimpLoader();

    bool LoadModel(std::string filename);
};


#endif //OPENGL_ASSIMP_H
