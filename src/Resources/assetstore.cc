#include <Opal/Resources/scenehandler.hh>
#include <Opal/Resources/assetstore.hh>
#include <Opal/Util/exceptions.hh>
#include <Opal/Util/log.hh>
#include <Opal/Resources/texture.hh>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <vector>
#include <fstream>

Opal::AssetStore::AssetStore(const std::string &scene) {

    Resources::SceneHandler sh{scene};

    for(auto &[name, texture] : sh.getTextures()) {
        m_textures.emplace(std::move(name), Texture(std::move(texture)));
    }

    auto &model3ds{sh.getModel3Ds()};

    for(auto &[name, m3d] : sh.getModel3Ds()) {
        std::unordered_map<std::string, Texture*> textures;
        for(auto &mesh : m3d.meshes) {
            if(auto iter = m_textures.find(mesh.matName); iter != m_textures.end()) {
                textures.emplace(mesh.matName, &iter->second);
            } else {
                throw BadResource{"Couldn't find texture " + mesh.matName, name};
            }
        }

        m_model3Ds.emplace(name, Model3D{std::move(m3d), std::move(textures)});
    }

    // TODO Create Entities from SceneHandler to ResourceHandler

}

Opal::Model3D &Opal::AssetStore::getModel3D(const std::string &name) {
    if(auto res{m_model3Ds.find(name)}; res != m_model3Ds.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Model3D " + name + " not found"};
    }
}

const Opal::Model3D &Opal::AssetStore::getModel3D(const std::string &name) const {
    if(auto res{m_model3Ds.find(name)}; res != m_model3Ds.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Model3D " + name + " not found"};
    }
}

Opal::Texture &Opal::AssetStore::getTexture(const std::string &name) {
    if(auto res{m_textures.find(name)}; res != m_textures.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Texture " + name + " not found"};
    }
}

const Opal::Texture &Opal::AssetStore::getTexture(const std::string &name) const {
    if(auto res{m_textures.find(name)}; res != m_textures.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Texture " + name + " not found"};
    }
}

Opal::Shader &Opal::AssetStore::getShader(const std::string &name) {
    if( auto res{m_shaders.find(name)}; res != m_shaders.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Shader " + name + " not found"};
    }
}

const Opal::Shader &Opal::AssetStore::getShader(const std::string &name) const {
    if( auto res{m_shaders.find(name)}; res != m_shaders.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Shader " + name + " not found"};
    }
}
