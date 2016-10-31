#ifndef OPENGL_ASSIMP_H
#define OPENGL_ASSIMP_H

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Render/Textures/texture.hpp>
#include <Resources/model3d.hpp>

namespace ResourceLoader {
    std::shared_ptr<Texture> LoadTexture(std::string filename, bool genMipMaps);
    std::shared_ptr<Model3D> LoadModel3D(std::string modelname);
}

#endif //OPENGL_ASSIMP_H
