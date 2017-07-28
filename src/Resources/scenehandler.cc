#include "scenehandler.hh"
#include "resources.hh"
#include "util.hh"

#include <json.hpp>

using json = nlohmann::json;

void Opal::Resources::SceneHandler::readFromJSON(std::ifstream &file) {
    std::cout << "Reading JSON file...\n";
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&contents[0], contents.size());
    file.close();

    json scene = json::parse(contents);

    if(auto jname = scene.find("name"); jname != scene.end()) {
        m_sceneName = *jname;
    } else {
        throw std::runtime_error("JSON scene doesn't contain a name");
    }

    if(auto jresources = scene.find("resources"); jresources != scene.end()) {
        for(auto &resource : *jresources) {
            if(!resource.count("resourcename")) {
                throw std::runtime_error("resource missing resourcename field");
            }

            std::string resourcename = resource["resourcename"];

            if(!resource.count("type")) {
                throw std::runtime_error(resourcename + " missing type field");
            }

            if(!resource.count("filename")) {
                throw std::runtime_error(resourcename + " missing filename field");
            }

            std::string type = resource["type"];
            std::string filename = resource["filename"];

            if(type == "model3d") {
                if(m_model3ds.find(resourcename) == m_model3ds.end()) {
                    auto [m3d, textures] = loadModel3D(filename);
                    if(auto [iter, placed] = m_model3ds.emplace(resourcename, std::move(m3d)); placed) {
                        for(auto &[name, texture] : textures) {
                            if(!addTexture(name, std::move(texture))) {
                                std::cerr << "Texture " << name << " already added, skipping\n";
                            }
                        }
                    } else {
                        std::cerr << "model3d " << resourcename << " already placed, skipping\n";
                    }
                } else {
                    std::cout << "Model3D " << resourcename << " already added, skipping...\n";
                }
            } else if(type == "texture") {
                if(m_textures.find(resourcename) == m_textures.end()) {
                    m_textures.emplace(resourcename, loadTexture(filename));
                }
            } else {
                std::cerr << "Unknown resource type " << type << '\n';
            }
        }
    } else {
        throw std::runtime_error("Scene contains no resources");
    }
    std::cout << "Done\n";
}

void Opal::Resources::SceneHandler::readFromBIN(std::ifstream &file) {
    std::cout << "Reading OPL file...\n";
    std::array<char, 3> tag = Opal::Util::read<decltype(tag)>(file);
    unsigned short version = Opal::Util::read<decltype(version)>(file);
    m_sceneName = Opal::Util::readString(file);
    std::cout << "Loading scene " << m_sceneName << '\n';

    std::size_t modelcount = Opal::Util::read<std::size_t>(file);
    for(auto i = 0u; i < modelcount; i++) {
        if(Opal::Util::read<char>(file) == Opal::Util::RES_MODEL3D) {
            auto [name, m3d] = loadModel3D(file);
            addModel3D(name, std::move(m3d));
        }
    }

    std::size_t texturecount = Opal::Util::read<std::size_t>(file);
    for(auto i = 0u; i < texturecount; i++) {
        if(Opal::Util::read<char>(file) == Opal::Util::RES_TEXTURE) {
            auto tex = loadTexture(file);
            addTexture(tex.name, std::move(tex));
        }
    }
    std::cout << "Done\n";
}

void Opal::Resources::SceneHandler::writeToJSON() {

}

void Opal::Resources::SceneHandler::writeToBIN () {
    std::cout << "Compiling to output.opl...\n";
    std::ofstream file("output.opl", std::ios::binary);
    file.write(Opal::Util::OPLTAG, sizeof(Opal::Util::OPLTAG));
    Opal::Util::write(file, Opal::Util::VERSION);
    Opal::Util::writeString(file, m_sceneName);
    std::cout << m_sceneName << '\n';

    Opal::Util::write(file, m_model3ds.size());
    for(auto &[name, model] : m_model3ds) {
        Opal::Util::write(file, Opal::Util::RES_MODEL3D);
        Opal::Util::writeString(file, name);
        file << model;
    }

    Opal::Util::write(file, m_textures.size());
    for(auto &[name, texture] : m_textures) {
        Opal::Util::write(file, Opal::Util::RES_TEXTURE);
        Opal::Util::writeString(file, name);
        file << texture;
    }
    std::cout << "Done\n";
}

void Opal::Resources::SceneHandler::deleteModel3D(const std::string &name) {
    if(auto iter = m_model3ds.find(name); iter != m_model3ds.end()) {
        m_model3ds.erase(iter);
    } else {
        throw std::runtime_error("Model3D " + name + " doesn't exist");
    }
}

void Opal::Resources::SceneHandler::deleteTexture(const std::string &name) {
    if(auto iter = m_textures.find(name); iter != m_textures.end()) {
        m_textures.erase(iter);
    } else {
        throw std::runtime_error("Texture " + name + " doesn't exist");
    }
}

void Opal::Resources::SceneHandler::info() {
    std::cout << "Texture count: " << m_textures.size() << '\n';
    for(auto &[name, texture] : m_textures) {
        std::cout << '\t' << name << " [" << texture.width << ',' << texture.height << "]\n";
    }
    std::cout << "Model3D count: " << m_model3ds.size() << '\n';
    for(auto &[name, model] : m_model3ds) {
        std::cout << '\t' << name << " [" << model.meshes.size() << "]\n";
    }
}

void Opal::Resources::SceneHandler::model3DInfo() {
    for(auto &model : m_model3ds) {
        std::cout << model.first << '\n';
        for(auto i = 0u; i < model.second.meshes.size(); i++) {
            auto &mesh = model.second.meshes[i];
            std::cout << "Mesh [" << i << "]\n";
            std::cout << '\t' << "Texture: " << mesh.matName << '\n';
            std::cout << '\t' << "Vertex Size: " << mesh.vertices.size() << '\n';
            std::cout << '\t' << "Index Size: " << mesh.indices.size() << "\n\n";
        }
    }
}

void Opal::Resources::SceneHandler::textureInfo() {
    for(auto &texture : m_textures) {
        std::cout << texture.first << '\n';
        std::cout << '\t' << "Width: " << texture.second.width << '\n';
        std::cout << '\t' << "Height: " << texture.second.height << '\n';
    }
}

std::unordered_map<std::string, Opal::Resources::RTexture> &Opal::Resources::SceneHandler::getTextures() {
    return m_textures;
}

std::unordered_map<std::string, Opal::Resources::RModel3D> &Opal::Resources::SceneHandler::getModel3Ds() {
    return m_model3ds;
}

bool Opal::Resources::SceneHandler::addModel3D(const std::string &name, RModel3D &&model) {
    auto [iter, placed] = m_model3ds.emplace(name, std::move(model));
    return placed;
}

bool Opal::Resources::SceneHandler::addTexture(const std::string &name, RTexture &&texture) {
    auto [iter, placed] = m_textures.emplace(name, std::move(texture));
    return placed;
}
