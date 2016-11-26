#ifndef OPENGL_ASSIMP_H
#define OPENGL_ASSIMP_H

#include <memory>
#include <Utilities/exceptions.hpp>
#include <Render/renderobject.hpp>
#include <Render/Textures/texture.hpp>
#include <Resources/model3d.hpp>
#include <json.hpp>

namespace ResourceLoader {
    std::shared_ptr<IRenderObject> LoadModelJSON(nlohmann::json object);
    std::shared_ptr<IRenderObject> LoadLineJSON(nlohmann::json object);
    std::shared_ptr<Texture> LoadTexture(std::string filename, bool genMipMaps);
    std::shared_ptr<Model3D> LoadModel3D(std::string modelname);
}

#endif //OPENGL_ASSIMP_H
