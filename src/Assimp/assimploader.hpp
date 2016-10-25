#ifndef OPENGL_ASSIMP_H
#define OPENGL_ASSIMP_H

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Model/Textures/texture.hpp>

#include "assimpmodel.hpp"

namespace AssimpLoader
{
    std::shared_ptr<Texture> LoadTexture(std::string filename);
    std::shared_ptr<AssimpModel> LoadModel(std::string filename);
}

#endif //OPENGL_ASSIMP_H
