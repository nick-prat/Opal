#ifndef _RESOURCE_HANDLER_H
#define _RESOURCE_HANDLER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include <Utilities/exceptions.hh>
#include <Render/shader.hh>
#include <Resources/model3d.hh>
#include <Resources/texture.hh>

// TODO Might be a good idea to store any resource in ogl buffers instead

namespace Opal {

    class AssetStore {
    public:
        AssetStore(const std::string &scene);

        Model3D &getModel3D(const std::string &name);
        const Model3D &getModel3D(const std::string &name) const;
        Texture &getTexture(const std::string &name);
        const Texture &getTexture(const std::string &name) const;
        Shader &getShader(const std::string &name);
        const Shader &getShader(const std::string &name) const;

    private:
        bool scan();

    private:
        std::unordered_map<std::string, Shader> m_shaders;
        std::unordered_map<std::string, Texture> m_textures;
        std::unordered_map<std::string, Model3D> m_model3Ds;
    };

}

#endif // _RESOURCE_HANDLER_H
