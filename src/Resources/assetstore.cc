#include <Resources/assetstore.hh>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <vector>
#include <fstream>

#include <Util/exceptions.hh>
#include <Util/log.hh>
#include <Resources/texture.hh>

#include "scenehandler.hh"

Opal::AssetStore::AssetStore(const std::string &scene) {

    std::ifstream file{scene};
    if(!file.is_open()) {
        throw std::runtime_error{"Couldn't open file " + scene};
    }

    Resources::SceneHandler sh;
    sh.readFromBIN(file);

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

        m_model3Ds.emplace(name, Model3D(std::move(m3d), std::move(textures)));
    }

    // TODO Create Entities from SceneHandler to ResourceHandler

}

/*
Shader &Opal::AssetStore::getShader(const std::string &shader) {
    auto ret = m_shaders.find(shader);
    if(ret != m_shaders.end()) {
        return ret->second;
    }
    throw BadResource("Couldn't find shader", shader);
}

void AssetStore::loadShader(const json &object) {
    std::string name = object["resourcename"];
    if(m_shaders.find(name) != m_shaders.end()) {
        return;
    }

    std::string filename = object["filename"];
    std::vector<std::string> files = object["types"];
    if(files.size() == 0) {
        throw BadResource("types is empty", name);
    }
    std::vector<GLenum> types;
    for(auto &file : files) {
        if(file == "fragment") {
            types.push_back(GL_FRAGMENT_SHADER);
            file = filename + '/' + filename + "_fs.glsl";
        } else if(file == "vertex") {
            types.push_back(GL_VERTEX_SHADER);
            file = filename + '/' + filename + "_vs.glsl";
        } else if(file == "geometry") {
            types.push_back(GL_GEOMETRY_SHADER);
            file = filename + '/' + filename + "_gs.glsl";
        }
    }

    Shader shader(files, types);

    if(object.find("uniforms") != object.end()) {
        std::vector<std::string> uniforms = object["uniforms"];
        for(const auto &uniform : uniforms) {
            shader.registerUniform(uniform);
        }
    }

    m_shaders.emplace(name, std::move(shader));
}
*/

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
