#include <Opal/Resources/scenehandler.hh>
#include <Opal/Resources/resources.hh>
#include <Opal/Util/util.hh>

#include <json.hpp>

using json = nlohmann::json;

Opal::Resources::SceneHandler::SceneHandler(std::istream &stream) {
    std::array<char, 3> tag;
    stream.read(tag.data(), 3);
    stream.seekg(std::ios::beg);

    if(Util::bytecmp<3>(tag.data(), Util::OPLTAG)) {
        readBIN(stream);
    } else {
        readJSON(stream);
    }
}

void Opal::Resources::SceneHandler::readJSON(std::istream &stream) {
    std::cout << "Reading JSON file...\n";
    std::string contents;
    stream.seekg(0, std::ios::end);
    std::cout << stream.tellg() << '\n';
    contents.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&contents[0], contents.size());

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
                    m3d.name = resourcename;
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
                    std::cerr << "Model3D " << resourcename << " already added, skipping...\n";
                }
            } else if(type == "texture") {
                if(m_textures.find(resourcename) == m_textures.end()) {
                    auto texture = loadTexture(filename);
                    texture.name = resourcename;
                    m_textures.emplace(resourcename, std::move(texture));
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

void Opal::Resources::SceneHandler::readBIN(std::istream &stream) {
    std::array<char, 3> tag = Opal::Util::read<decltype(tag)>(stream);
    unsigned short version = Opal::Util::read<decltype(version)>(stream);
    m_sceneName = Opal::Util::readString(stream);

    std::size_t modelcount = Opal::Util::read<std::size_t>(stream);
    for(auto i = 0u; i < modelcount; i++) {
        if(Opal::Util::read<char>(stream) == Util::ResType::Model3D) {
            auto [name, m3d] = loadModel3D(stream);
            addModel3D(name, std::move(m3d));
        }
    }

    std::size_t texturecount = Opal::Util::read<std::size_t>(stream);
    for(auto i = 0u; i < texturecount; i++) {
        if(Opal::Util::read<char>(stream) == Util::ResType::Texture) {
            auto tex = loadTexture(stream);
            addTexture(tex.name, std::move(tex));
        }
    }
}

void Opal::Resources::SceneHandler::writeToJSON(std::ostream &stream) {

}

void Opal::Resources::SceneHandler::writeToBIN(std::ostream &stream) {
    Opal::Util::write(stream, Opal::Util::OPLTAG, 3);
    Opal::Util::write(stream, Opal::Util::VERSION);
    Opal::Util::writeString(stream, m_sceneName);

    Opal::Util::write(stream, m_model3ds.size());
    for(auto &[name, model] : m_model3ds) {
        stream << model;
    }

    Opal::Util::write(stream, m_textures.size());
    for(auto &[name, texture] : m_textures) {
        stream << texture;
    }
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
