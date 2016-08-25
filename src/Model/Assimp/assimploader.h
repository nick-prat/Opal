//
// Created by nprat on 7/14/16.
//

#ifndef OPENGL_ASSIMP_H
#define OPENGL_ASSIMP_H

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assimpmodel.h"

namespace AssimpLoader
{
    std::unique_ptr<AssimpModel> LoadModel(std::string filename);
};


#endif //OPENGL_ASSIMP_H
