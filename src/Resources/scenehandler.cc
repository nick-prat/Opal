#include <Opal/Resources/scenehandler.hh>
#include <Opal/Resources/resources.hh>
#include <Opal/Util/util.hh>

#include <json.hpp>

using json = nlohmann::json;

Opal::Resources::SceneHandler::SceneHandler(const std::string& scene)
: m_sceneName(scene) {
    std::ifstream file{scene + ".json"};
    if(file.is_open()) {
        read(file);
    } else {
        throw std::runtime_error("Couldn't load file " + m_sceneName + ".json");
    }
}

Opal::Resources::SceneHandler::SceneHandler(std::istream& stream) {
    read(stream);
}

Opal::Resources::SceneHandler& Opal::Resources::SceneHandler::operator=(SceneHandler&& scene) noexcept {
    m_sceneName = std::move(scene.m_sceneName);
    m_textures = std::move(scene.m_textures);
    m_model3ds = std::move(scene.m_model3ds);
    return *this;
}

void Opal::Resources::SceneHandler::read(std::istream& stream) {
    m_textures.clear();
    m_model3ds.clear();

    std::array<char, 3> tag;
    stream.read(tag.data(), 3);
    stream.seekg(std::ios::beg);

    if(Util::bytecmp<3>(tag.data(), Util::OPLTAG)) {
        readBIN(stream);
    } else {
        readJSON(stream);
    }
}

void Opal::Resources::SceneHandler::readJSON(std::istream& stream) {
    std::cout << "Reading JSON file...\n";
    std::string contents;
    stream.seekg(0, std::ios::end);
    contents.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&contents[0], contents.size());

    json scene = json::parse(contents);

    if(auto jname = scene.find("name"); jname != scene.end()) {
        m_sceneName = *jname;
    } else {
        throw std::runtime_error("JSON missing property \'name\'");
    }

    if(auto jresources = scene.find("resources"); jresources != scene.end()) {
        try {
            for(auto& resource : *jresources) {
                if(resource.find("resourcename") == resource.end()) {
                    throw std::runtime_error("resource missing property \'field\'");
                }

                const std::string resourcename = resource["resourcename"];

                if(resource.find("filename") == resource.end()) {
                    throw std::runtime_error("Resource " + resourcename + " missing property \'filename\'");
                }

                const std::string filename = resource["filename"];

                if(resource.find("type") == resource.end()) {
                    throw std::runtime_error("Resource " + resourcename + " missing property \'type\'");
                }

                const std::string type = resource["type"];

                if(type == "model3d") {
                    if(m_model3ds.find(resourcename) == m_model3ds.end()) {
                        const std::string filepath = "Resources/Models/" + filename + ".3ds";
                        auto [m3d, texturenames] = loadModel3D(filepath, resourcename);
                        if(const auto [iter, placed] = m_model3ds.emplace(resourcename, std::move(m3d)); placed) {
                            for(const auto& texture : texturenames) {
                                const auto texresourcename{resourcename + "_" + texture};
                                const auto filename{"Resources/Textures/" + resourcename + "/" + texture + ".tga"};
                                try {
                                    if(!addTexture(loadTexture(filename, texresourcename))) {
                                        std::cerr << "Texture " << texresourcename << " already added, skipping\n";
                                    }
                                } catch (const std::runtime_error &error) {
                                    std::cerr << texresourcename << " failed to load: " << error.what() << " skipping\n";
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
                        const std::string filepath{"Resources/Textures/" + filename + ".tga"};
                        if(!addTexture(loadTexture(filepath, resourcename))) {
                            std::cerr << "Texture " << resourcename << " already added, skipping\n";
                        }
                    }
                } else if(type == "shader") {
                    if(m_shaders.find(resourcename) == m_shaders.end()) {
                        if(!addShader(loadShader(resource))) {
                            std::cerr << "Shader " << resourcename << " already added, skipping\n";
                        }
                    }
                } else {
                    std::cerr << "Unknown resource type " << type << '\n';
                }
            }
        } catch(std::runtime_error& error) {
            std::cerr << "Error parsing scene.json: " << error.what() << '\n';
        }
    }
    std::cout << "Done\n";
}

void Opal::Resources::SceneHandler::readBIN(std::istream& stream) {
    auto tag{Opal::Util::read<std::array<char, 3>>(stream)};
    auto version{Opal::Util::read<unsigned short>(stream)};
    m_sceneName = Opal::Util::readString(stream);

    auto modelcount{Opal::Util::read<std::size_t>(stream)};
    for(auto i{0u}; i < modelcount; i++) {
        if(Opal::Util::read<char>(stream) == ResType::Model3D) {
            auto m3d = loadModel3D(stream);
            addModel3D(std::move(m3d));
        }
    }

    auto texturecount{Opal::Util::read<std::size_t>(stream)};
    for(auto i{0u}; i < texturecount; i++) {
        if(Opal::Util::read<char>(stream) == ResType::Texture2D) {
            auto tex = loadTexture(stream);
            addTexture(std::move(tex));
        }
    }
}

void Opal::Resources::SceneHandler::writeToJSON(std::ostream& stream) {
    json scene;
    scene["name"] = m_sceneName;
    scene["resources"] = std::vector<json>{};

    for(const auto& [name, m3d] : m_model3ds) {
        json jm3d;
        jm3d["type"] = "model3d";
        jm3d["resourcename"] = m3d.name;
        jm3d["filename"] = m3d.filename;
        scene["resources"].push_back(jm3d);
    }

    for(const auto& [name, texture] : m_textures) {
        json jtex;
        jtex["type"] = "texture";
        jtex["resourcename"] = texture.name;
        jtex["filename"] = texture.filename;
        scene["resources"].push_back(jtex);
    }

    stream << scene.dump(4);
}

void Opal::Resources::SceneHandler::writeToBIN(std::ostream& stream) {
    stream.write(Opal::Util::OPLTAG, 3);
    Opal::Util::write(stream, Opal::Util::VERSION);
    Opal::Util::writeString(stream, m_sceneName);

    Opal::Util::write(stream, m_model3ds.size());
    for(auto& [name, model] : m_model3ds) {
        stream << model;
    }

    Opal::Util::write(stream, m_textures.size());
    for(auto& [name, texture] : m_textures) {
        stream << texture;
    }
}

void Opal::Resources::SceneHandler::deleteModel3D(const std::string& name) {
    if(auto iter = m_model3ds.find(name); iter != m_model3ds.end()) {
        m_model3ds.erase(iter);
    } else {
        throw std::runtime_error("Model3D " + name + " doesn't exist");
    }
}

void Opal::Resources::SceneHandler::deleteTexture(const std::string& name) {
    if(auto iter = m_textures.find(name); iter != m_textures.end()) {
        m_textures.erase(iter);
    } else {
        throw std::runtime_error("Texture " + name + " doesn't exist");
    }
}

void Opal::Resources::SceneHandler::info() {
    std::cout << "Texture count: " << m_textures.size() << '\n';
    for(auto& [name, texture] : m_textures) {
        std::cout << '\t' << name << " [" << texture.width << ',' << texture.height << "]\n";
    }
    std::cout << "Model3D count: " << m_model3ds.size() << '\n';
    for(auto& [name, model] : m_model3ds) {
        std::cout << '\t' << name << " [" << model.meshes.size() << "]\n";
    }
}

void Opal::Resources::SceneHandler::model3DInfo() {
    for(auto& model : m_model3ds) {
        std::cout << model.first << '\n';
        for(auto i = 0u; i < model.second.meshes.size(); i++) {
            auto& mesh = model.second.meshes[i];
            std::cout << "Mesh [" << i << "]\n";
            std::cout << '\t' << "Texture: " << mesh.matName << '\n';
            std::cout << '\t' << "Vertex Size: " << mesh.vertices.size() << '\n';
            std::cout << '\t' << "Index Size: " << mesh.indices.size() << "\n\n";
        }
    }
}

void Opal::Resources::SceneHandler::textureInfo() {
    for(auto& texture : m_textures) {
        std::cout << texture.first << '\n';
        std::cout << '\t' << "Width: " << texture.second.width << '\n';
        std::cout << '\t' << "Height: " << texture.second.height << '\n';
    }
}

std::unordered_map<std::string, Opal::Resources::RTexture>& Opal::Resources::SceneHandler::getTextures() {
    return m_textures;
}

std::unordered_map<std::string, Opal::Resources::RModel3D>& Opal::Resources::SceneHandler::getModel3Ds() {
    return m_model3ds;
}

std::unordered_map<std::string, Opal::Resources::RShader>& Opal::Resources::SceneHandler::getShaders() {
    return m_shaders;
}

bool Opal::Resources::SceneHandler::addModel3D(RModel3D&& model) {
    auto [iter, placed] = m_model3ds.emplace(model.name, std::move(model));
    return placed;
}

bool Opal::Resources::SceneHandler::addTexture(RTexture&& texture) {
    auto [iter, placed] = m_textures.emplace(texture.name, std::move(texture));
    return placed;
}

bool Opal::Resources::SceneHandler::addShader(RShader&& shader) {
    auto [iter, placed] = m_shaders.emplace(shader.name, std::move(shader));
    return placed;
}

std::string Opal::Resources::SceneHandler::getSceneName() {
    return m_sceneName;
}
