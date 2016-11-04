#ifndef OPENGL_ASSIMP_H
#define OPENGL_ASSIMP_H

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Display/display.hpp>
#include <Render/renderobject.hpp>
#include <Render/Textures/texture.hpp>
#include <Resources/model3d.hpp>

namespace ResourceLoader {
    std::vector<std::shared_ptr<IRenderObject>> LoadScene(std::shared_ptr<GlutDisplay> display, std::string filename);
    std::shared_ptr<Texture> LoadTexture(std::string filename, bool genMipMaps);
    std::shared_ptr<Model3D> LoadModel3D(std::string modelname);
}

#endif //OPENGL_ASSIMP_H
