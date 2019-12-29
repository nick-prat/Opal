#include <Opal/Resources/scenehandler.hh>
#include <Opal/Resources/assetstore.hh>
#include <Opal/Util/exceptions.hh>
#include <Opal/Util/log.hh>
#include <Opal/Resources/texture.hh>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <initializer_list>

Opal::AssetStore::AssetStore(const std::string& scene) {
    Resources::SceneHandler sh{scene};

    for(auto& [name, texture] : sh.getTextures()) {
        m_textures.emplace(std::move(name), Texture(std::move(texture)));
    }

    for(auto& [name, m3d] : sh.getModel3Ds()) {
        std::unordered_map<std::string, Texture*> textures;
        for(auto& mesh : m3d.meshes) {
            if(auto iter = m_textures.find(mesh.matName); iter != m_textures.end()) {
                textures.emplace(mesh.matName, &iter->second);
            } else {
                throw BadResource{"Couldn't find texture " + mesh.matName, name};
            }
        }
        m_model3Ds.emplace(name, Model3D{m3d, std::move(textures)});
    }

    for(auto& [name, shader] : sh.getShaders()) {
        m_shaders.emplace(name, shader);
    }

    for(auto const& [name, terrain] : sh.getTerrains()) {
        m_terrainPatches.emplace(name, terrain);
    }

    // TODO Create Entities from SceneHandler to ResourceHandler

}

Opal::Model3D& Opal::AssetStore::getModel3D(const std::string& name) {
    if(auto res{m_model3Ds.find(name)}; res != m_model3Ds.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Model3D " + name + " not found"};
    }
}

const Opal::Model3D& Opal::AssetStore::getModel3D(const std::string& name) const {
    if(auto res{m_model3Ds.find(name)}; res != m_model3Ds.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Model3D " + name + " not found"};
    }
}

Opal::Texture& Opal::AssetStore::getTexture(const std::string& name) {
    if(auto res{m_textures.find(name)}; res != m_textures.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Texture " + name + " not found"};
    }
}

const Opal::Texture& Opal::AssetStore::getTexture(const std::string& name) const {
    if(auto res{m_textures.find(name)}; res != m_textures.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Texture " + name + " not found"};
    }
}

Opal::Shader& Opal::AssetStore::getShader(const std::string& name) {
    if( auto res{m_shaders.find(name)}; res != m_shaders.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Shader " + name + " not found"};
    }
}

const Opal::Shader& Opal::AssetStore::getShader(const std::string& name) const {
    if( auto res{m_shaders.find(name)}; res != m_shaders.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Shader " + name + " not found"};
    }
}

Opal::TerrainPatch& Opal::AssetStore::getTerrainPatch(std::string const& name) {
    if( auto res{m_terrainPatches.find(name)}; res != m_terrainPatches.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Terrain Patch " + name + " not found"};
    }
}

Opal::TerrainPatch const& Opal::AssetStore::getTerrainPatch(std::string const& name) const {
    if( auto res{m_terrainPatches.find(name)}; res != m_terrainPatches.end()) {
        return res->second;
    } else {
        throw std::invalid_argument{"Terrain Patch " + name + " not found"};
    } 
}

std::unordered_map<std::string, Opal::Shader>& Opal::AssetStore::getShaders() {
    return m_shaders;
}

const std::unordered_map<std::string, Opal::Shader>& Opal::AssetStore::getShaders() const {
    return m_shaders;
}

std::unordered_map<std::string, Opal::Texture>& Opal::AssetStore::getTextures() {
    return m_textures;
}

const std::unordered_map<std::string, Opal::Texture>& Opal::AssetStore::getTextures() const {
    return m_textures;
}

std::unordered_map<std::string, Opal::Model3D>& Opal::AssetStore::getModel3Ds() {
    return m_model3Ds;
}

const std::unordered_map<std::string, Opal::Model3D>& Opal::AssetStore::getModel3Ds() const {
    return m_model3Ds;
}
