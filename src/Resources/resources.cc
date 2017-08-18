#include <Opal/Resources/resources.hh>
#include <Opal/Util/util.hh>

#include <FreeImage.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

using namespace Opal::Util;
using namespace Opal::Resources;
using json = nlohmann::json;

Opal::Resources::RFile::RFile(RFile &&file)
: bytes{std::move(file.bytes)} {}

Opal::Resources::RFile::RFile(std::istream &stream) {
    auto size{read<std::size_t>(stream)};
    bytes.resize(size);
    stream.read(bytes.data(), bytes.size());
}

Opal::Resources::RFile::RFile(std::vector<char> &&bytes)
: bytes{std::move(bytes)} {}

Opal::Resources::RVertex::RVertex(std::istream &stream) {
    position = read<decltype(position)>(stream);
    normal = read<decltype(normal)>(stream);
    texCoord = read<decltype(texCoord)>(stream);
}

Opal::Resources::RVertex::RVertex(RVertex &&vert)
: position{vert.position}
, normal{vert.normal}
, texCoord{vert.texCoord} {}

Opal::Resources::RVertex::RVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tc)
: position{pos}
, normal{norm}
, texCoord{tc} {}

Opal::Resources::RMesh::RMesh(std::istream &stream) {
    auto size{read<std::size_t>(stream)};

    stream >> matIndex;
    matName = readString(stream);

    auto vsize{read<std::size_t>(stream)};
    vertices.resize(vsize);
    stream.read((char*)vertices.data(), sizeof(RVertex) * vsize);

    auto isize{read<std::size_t>(stream)};
    indices.resize(isize);
    stream.read((char*)indices.data(), sizeof(unsigned int) * isize);
}

Opal::Resources::RMesh::RMesh(RMesh &&mesh)
: matIndex{mesh.matIndex}
, matName{std::move(mesh.matName)}
, vertices{std::move(mesh.vertices)}
, indices{std::move(mesh.indices)} {}

Opal::Resources::RMesh::RMesh(std::vector<RVertex> &&verts, std::vector<unsigned int> &&inds)
: vertices{std::move(verts)}
, indices{std::move(inds)} {}

Opal::Resources::RModel3D::RModel3D(RModel3D &&m3d)
: name{std::move(m3d.name)}
, filename{std::move(m3d.filename)}
, meshes{std::move(m3d.meshes)} {}

Opal::Resources::RModel3D::RModel3D(std::vector<RMesh> &&meshes_)
: meshes{std::move(meshes_)} {}

Opal::Resources::RTexture::RTexture(RTexture &&texture)
: name{std::move(texture.name)}
, filename{std::move(texture.filename)}
, bytes{std::move(texture.bytes)}
, width{texture.width}
, height{texture.height} {}

Opal::Resources::RTexture::RTexture(std::istream &stream) {
    name = readString(stream);
    width = read<decltype(width)>(stream);
    height = read<decltype(height)>(stream);
    bytes.reserve(width * height * Util::RES_TEXTURE_BPP);
    stream.read(bytes.data(), bytes.size());
}

Opal::Resources::RTexture::RTexture(std::vector<char> &&bytes, unsigned int width, unsigned int height)
: bytes{std::move(bytes)}
, width{width}
, height{height} {}

Opal::Resources::RShader::RShader(RShader &&shader)
: name{shader.name}
, files{std::move(shader.files)} {}

Opal::Resources::RShader::RShader(std::istream &stream) {
    auto base{stream.tellg()};
    name = readString(stream);
    auto size{read<std::size_t>(stream)};
    for(auto i{0u}; i < size; i++) {
        auto type{read<char>(stream)};
        auto size{read<std::size_t>(stream)};
        if(auto fileIter = files.emplace(type, RFile{}); fileIter.second) {
            stream.read(fileIter.first->second.bytes.data(), size);
        } else {
            stream.seekg(base);
            throw std::runtime_error("Shader " + name + " contains multiple shaders of same type");
        }
    }
}

Opal::Resources::RShader::RShader(std::unordered_map<char, RFile> &&files)
: files{std::move(files)} {}

Opal::Resources::RObject::RObject(std::istream &stream) {

}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RModel3D &model3d) {
    write(stream, Opal::Util::ResType::Model3D);
    write(stream, sizeOf(model3d));
    writeString(stream, model3d.name);
    write(stream, model3d.meshes.size());
    for(auto& mesh : model3d.meshes) {
        stream << mesh;
    }
    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RMesh &mesh) {
    write(stream, sizeOf(mesh));
    write(stream, mesh.matIndex);
    writeString(stream, mesh.matName);

    write(stream, mesh.vertices.size());
    stream.write((char*)mesh.vertices.data(), mesh.vertices.size() * sizeof(RVertex));

    write(stream, mesh.indices.size());
    stream.write((char*)mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RTexture &texture) {
    write(stream, Opal::Util::ResType::Texture);
    write(stream, sizeOf(texture));
    writeString(stream, texture.name);
    write(stream, texture.width);
    write(stream, texture.height);
    stream.write(texture.bytes.data(), texture.bytes.size());
    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RShader &shader) {
    write(stream, Opal::Util::ResType::Shader);
    write(stream, sizeOf(shader));

    writeString(stream, shader.name);
    write(stream, shader.files.size());
    for(const auto &[type, file] : shader.files) {
        write(stream, type);
        write(stream, file.bytes.size());
        stream.write(file.bytes.data(), file.bytes.size());
    }
    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RFile &file) {
    write(stream, Opal::Util::ResType::File);
    write(stream, sizeOf(file));
    stream.write(file.bytes.data(), file.bytes.size());
    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RObject &object) {
    write(stream, object.type);
    writeString(stream, object.resourceName);
    write(stream, object.position);
    write(stream, object.rotation);
    write(stream, object.scale);
    return stream;
}

std::size_t Opal::Resources::sizeOf(const RFile &file) {
    return file.bytes.size();
}

std::size_t Opal::Resources::sizeOf(const RModel3D &model3d) {
    std::size_t size{model3d.name.size() + 1};
    for(auto &mesh : model3d.meshes) {
        size += sizeOf(mesh);
    }
    return size;
}

std::size_t Opal::Resources::sizeOf(const RMesh &mesh) {
    return sizeof(unsigned int)
        + sizeof(sizeOf(mesh))
        + sizeof(mesh.matIndex)
        + mesh.matName.size() + 1
        + sizeof(mesh.vertices.size())
        + sizeof(RVertex) * mesh.vertices.size()
        + sizeof(mesh.indices.size())
        + sizeof(unsigned int) * mesh.indices.size();
}

std::size_t Opal::Resources::sizeOf(const RTexture &texture) {
    return texture.name.size() + 1
        + sizeof(decltype(texture.width))
        + sizeof(decltype(texture.height))
        + texture.bytes.size() * RES_TEXTURE_BPP;
}

std::size_t Opal::Resources::sizeOf(const RShader &shader) {
    std::size_t size{shader.name.size() + 1};
    for(auto &[type, file] : shader.files) {
        size += file.bytes.size();
    }
    return size;
}

std::size_t Opal::Resources::sizeOf(const RObject &object) {
    return sizeof(object.type)
        + object.resourceName.size()
        + sizeof(object.position)
        + sizeof(object.rotation)
        + sizeof(object.scale);
}

void loadNode(const aiScene* scene, const aiNode* node, const glm::mat4 &parentTransform, std::vector<RMesh> &meshes) {
    glm::mat4x4 transformation;
    copyaiMat(&node->mTransformation, transformation);
    transformation = parentTransform * transformation;

    for(auto i{0u}; i < node->mNumChildren; i++) {
        loadNode(scene, node->mChildren[i], transformation, meshes);
    }

    for(auto i{0u}; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<RVertex> vertices;
        std::vector<unsigned int> indices;

        for(auto j{0u}; j < mesh->mNumVertices; j++) {
            RVertex vertex;

            glm::vec4 position{transformation * glm::vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f)};
            vertex.position = glm::vec3{position.x, position.y, position.z};

            vertex.normal = (mesh->HasNormals())
                ? glm::vec3{mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z}
                : glm::vec3{0.0f, 0.0f, 0.0f};

            vertex.texCoord = (mesh->HasTextureCoords(0))
                ? glm::vec2{mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y}
                : glm::vec2{0.0f, 0.0f};

            vertices.push_back(vertex);
        }

        if(mesh->HasFaces()) {
            for(auto j = 0u; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for(auto k = 0u; k < face.mNumIndices; k++) {
                    indices.emplace_back(face.mIndices[k]);
                }
            }
        } else {
            std::cerr << "Node was missing faces, quitting...\n";
            exit(-1);
        }

        meshes.emplace_back(std::move(vertices), std::move(indices));
        meshes.back().matIndex = mesh->mMaterialIndex;
    }
}

RFile Opal::Resources::loadFile(const std::string &filename) {
    std::ifstream file{filename};
    file.seekg(std::ios::end);
    auto size{file.tellg()};
    file.seekg(std::ios::beg);
    std::vector<char> bytes;
    bytes.resize(size);
    file.read(bytes.data(), size);
    return {std::move(bytes)};
}

std::pair<RModel3D, std::unordered_map<std::string, RTexture>> Opal::Resources::loadModel3D(const std::string &filename, const std::string &resourcename) {
    Assimp::Importer importer;
    const aiScene* scene{importer.ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType)};

    std::vector<RMesh> meshes;

    if(!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error{"Loading model " + filename + " failed"};
        exit(-1);
    }

    std::unordered_map<std::string, RTexture> textures;

    aiMaterial** materials{scene->mMaterials};
    for(auto i{0u}; i < scene->mNumMaterials; i++) {
        aiString aName;
        materials[i]->Get(AI_MATKEY_NAME, aName);
        auto texname{std::string(aName.C_Str())};
        auto texresname{resourcename + "_" + texname};
        auto texfilename{resourcename + "/" + texname};
        if(textures.find(texresname) == textures.end()) {
            try {
                RTexture texture = loadTexture(texfilename);
                texture.name = texresname;
                texture.filename = texfilename;
                textures.emplace(texresname, std::move(texture));
            } catch(std::runtime_error &error) {
                std::cerr << error.what() << '\n';
            }
        }
    }

    loadNode(scene, scene->mRootNode, glm::mat4(1.0f), meshes);

    for(auto &mesh : meshes) {
        aiString aName;
        unsigned int index{mesh.matIndex};
        if(index < scene->mNumMaterials) {
            materials[mesh.matIndex]->Get(AI_MATKEY_NAME, aName);
            std::string resname{resourcename + "_" + aName.C_Str()};
            if(auto iter{textures.find(resname)}; iter != textures.end()) {
                mesh.matName = iter->first;
            } else {
                throw std::runtime_error(resourcename + " missing unknown texture " + resname);
            }
        }
    }

    RModel3D m3d{std::move(meshes)};
    m3d.filename = filename;
    m3d.name = resourcename;
    return {std::move(m3d), std::move(textures)};
}

RModel3D Opal::Resources::loadModel3D(std::istream &stream) {
    RModel3D m3d;

    auto modelSize{read<std::size_t>(stream)};
    m3d.name = readString(stream);
    auto meshSize{read<std::size_t>(stream)};

    for(auto i = 0u; i < meshSize; i++) {
        m3d.meshes.emplace_back(loadMesh(stream));
    }

    return m3d;
}

RMesh Opal::Resources::loadMesh(std::istream &stream) {
    RMesh mesh;
    auto size{read<std::size_t>(stream)};
    mesh.matIndex = read<decltype(mesh.matIndex)>(stream);
    mesh.matName = readString(stream);

    auto vertexCount{read<std::size_t>(stream)};
    mesh.vertices.resize(vertexCount);
    stream.read((char*)mesh.vertices.data(), mesh.vertices.size() * sizeof(RVertex));

    auto indexCount{read<std::size_t>(stream)};
    mesh.indices.resize(indexCount);
    stream.read((char*)mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

    return mesh;
}

RTexture Opal::Resources::loadTexture(const std::string &filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    if(!FreeImage_FIFSupportsReading(format)) {
        throw std::runtime_error(filename + " couldn't be read by FreeImage");
    }

    if(format == FIF_UNKNOWN) {
        throw std::runtime_error(filename + " has unknown format");
    }

    FIBITMAP *img = FreeImage_Load(format, filename.c_str());

    if(!img) {
        throw std::runtime_error(filename + " image data couldn't be loaded");
    }

    if(FreeImage_GetBPP(img) != 32) {
        FIBITMAP* oldImg = img;
        img = FreeImage_ConvertTo32Bits(oldImg);
        FreeImage_Unload(oldImg);
    }

    unsigned int height, width;
    width = FreeImage_GetWidth(img);
    height = FreeImage_GetHeight(img);

    unsigned char* bytes = FreeImage_GetBits(img);

    if(bytes == nullptr) {
        FreeImage_Unload(img);
        throw std::runtime_error("couldn't load image bytes for " + filename);
    }

    std::vector<char> vb{bytes, bytes + width * height * RES_TEXTURE_BPP};
    FreeImage_Unload(img);
    return {std::move(vb), width, height};
}

RTexture Opal::Resources::loadTexture(std::istream &stream) {
    RTexture texture;
    auto size{read<std::size_t>(stream)};
    texture.name = readString(stream);
    texture.width = read<decltype(texture.width)>(stream);
    texture.height = read<decltype(texture.height)>(stream);
    texture.bytes.resize(texture.width * texture.height * RES_TEXTURE_BPP);
    stream.read(texture.bytes.data(), texture.bytes.size());
    return texture;
}

RShader Opal::Resources::loadShader(const json &shader) {
    auto resourceNameIter{shader.find("resourcename")};
    if(resourceNameIter == shader.end()) {
        throw std::runtime_error("Shader missing property resourcename");
    }
    auto resourceName{resourceNameIter->get<std::string>()};

    auto fileNameIter{shader.find("filename")};
    if(fileNameIter == shader.end()) {
        throw std::runtime_error("Shader " + resourceName + " missing property filename");
    }
    auto filename{fileNameIter->get<std::string>()};

    std::vector<std::string> files{shader["types"]};
    if(files.size() == 0) {

    }

    std::unordered_map<char, RFile> shaderFiles;
    for(auto &file : files) {
        if(file == "fragment") {
            shaderFiles.emplace(ResType::ShaderFrag, Opal::Resources::loadFile("Shaders/" + filename + '/' + filename + "_fs.glsl"));
        } else if(file == "vertex") {
            shaderFiles.emplace(ResType::ShaderVert, loadFile("Shaders/" + filename + '/' + filename + "_vs.glsl"));
        } else if(file == "geometry") {
            shaderFiles.emplace(ResType::ShaderGeo, loadFile("Shaders/" + filename + '/' + filename + "_gs.glsl"));
        }
    }

    /*if(object.find("uniforms") != object.end()) {
        std::vector<std::string> uniforms = object["uniforms"];
        for(const auto &uniform : uniforms) {
            shader.registerUniform(uniform);
        }
    }*/

    return {std::move(shaderFiles)};
}

RShader Opal::Resources::loadShader(std::istream &stream) {
    RShader shader;

    return shader;
}
