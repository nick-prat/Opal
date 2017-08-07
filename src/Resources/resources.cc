#include <Opal/Resources/resources.hh>
#include <Opal/Util/util.hh>

#include <FreeImage.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

using namespace Opal::Util;

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
, meshes{std::move(m3d.meshes)} {}

Opal::Resources::RModel3D::RModel3D(std::vector<RMesh> &&meshes_)
: meshes{std::move(meshes_)} {}

Opal::Resources::RTexture::RTexture(RTexture &&texture)
: name{std::move(texture.name)}
, bytes{std::move(texture.bytes)}
, width{texture.width}
, height{texture.height} {}

Opal::Resources::RTexture::RTexture(std::istream &stream) {
    name = readString(stream);
    width = read<decltype(width)>(stream);
    height = read<decltype(height)>(stream);
    bytes.reserve(width * height * Util::RES_TEXTURE_BPP);
    stream.read((char*)bytes.data(), bytes.size());
}

Opal::Resources::RTexture::RTexture(std::vector<unsigned char> &&bytes, unsigned int width, unsigned int height)
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
        if(auto iter = files.emplace(type, std::vector<char>{}); iter.second) {
            stream.read(iter.first->second.data(), size);
        } else {
            stream.seekg(base);
            throw std::runtime_error("Shader " + name + " contains multiple shaders of same type");
        }
    }
}

Opal::Resources::RShader::RShader(std::unordered_map<char, std::vector<char>> files)
: files{std::move(files)} {}

Opal::Resources::Object::Object(std::istream &stream) {

}

std::size_t Opal::Resources::Object::size() const {
    return sizeof(type)
        + resourceName.size()
        + sizeof(position)
        + sizeof(rotation)
        + sizeof(scale);
}

void Opal::Resources::Object::writeToStream(std::ostream &stream) {
    write(stream, type);
    writeString(stream, resourceName);
    write(stream, position);
    write(stream, rotation);
    write(stream, scale);
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RModel3D &model3d) {
    write(stream, Opal::Util::ResType::Model3D);
    write(stream, size(model3d));
    writeString(stream, model3d.name);
    write(stream, model3d.meshes.size());
    for(auto& mesh : model3d.meshes) {
        stream << mesh;
    }
    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RMesh &mesh) {
    write(stream, size(mesh));
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
    write(stream, size(texture));
    writeString(stream, texture.name);
    write(stream, texture.width);
    write(stream, texture.height);
    write(stream, texture.bytes.data(), texture.bytes.size());
    return stream;
}

std::ostream &Opal::Resources::operator<<(std::ostream &stream, const RShader &shader) {
    write(stream, Opal::Util::ResType::Shader);
    write(stream, size(shader));

    writeString(stream, shader.name);
    write(stream, shader.files.size());
    for(const auto &[type, bytes] : shader.files) {
        write(stream, type);
        write(stream, bytes.size());
        write(stream, bytes.data(), bytes.size());
    }
    return stream;
}

std::size_t Opal::Resources::size(const RModel3D &model3d) {
    std::size_t size{model3d.name.size() + 1};
    for(auto &mesh : model3d.meshes) {
        size += Opal::Resources::size(mesh);
    }
    return size;
}

std::size_t Opal::Resources::size(const RMesh &mesh) {
    return sizeof(unsigned int)
        + sizeof(size(mesh))
        + sizeof(mesh.matIndex)
        + mesh.matName.size() + 1
        + sizeof(mesh.vertices.size())
        + sizeof(RVertex) * mesh.vertices.size()
        + sizeof(mesh.indices.size())
        + sizeof(unsigned int) * mesh.indices.size();
}

std::size_t Opal::Resources::size(const RTexture &texture) {
    return texture.name.size() + 1
        + sizeof(decltype(texture.width))
        + sizeof(decltype(texture.height))
        + texture.bytes.size() * RES_TEXTURE_BPP;
}

std::size_t Opal::Resources::size(const RShader &shader) {
    std::size_t size{shader.name.size() + 1};
    for(auto &[type, bytes] : shader.files) {
        size += bytes.size();
    }
    return size;
}

void loadNode(const aiScene* scene, const aiNode* node, const glm::mat4 &parentTransform, std::vector<Opal::Resources::RMesh> &meshes) {
    glm::mat4x4 transformation;
    copyaiMat(&node->mTransformation, transformation);
    transformation = parentTransform * transformation;

    for(auto i{0u}; i < node->mNumChildren; i++) {
        loadNode(scene, node->mChildren[i], transformation, meshes);
    }

    for(auto i{0u}; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<Opal::Resources::RVertex> vertices;
        std::vector<unsigned int> indices;

        for(auto j{0u}; j < mesh->mNumVertices; j++) {
            Opal::Resources::RVertex vertex;

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

std::pair<Opal::Resources::RModel3D, std::unordered_map<std::string, Opal::Resources::RTexture>> Opal::Resources::loadModel3D(const std::string &modelname) {
    std::string filename{"Resources/Models/" + modelname + ".3ds"};
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
        auto texresname{modelname + "_" + texname};
        auto texfilename{modelname + "/" + texname};
        if(textures.find(texresname) == textures.end()) {
            try {
                RTexture texture = loadTexture(texfilename);
                texture.name = texresname;
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
            std::string resname{modelname + "_" + aName.C_Str()};
            if(auto iter{textures.find(resname)}; iter != textures.end()) {
                mesh.matName = iter->first;
            } else {
                throw std::runtime_error(modelname + " missing unknown texture " + resname);
            }
        }
    }
    RModel3D m3d{std::move(meshes)};
    return {std::move(m3d), std::move(textures)};
}

Opal::Resources::RModel3D Opal::Resources::loadModel3D(std::istream &stream) {
    RModel3D m3d;

    auto modelSize{read<std::size_t>(stream)};
    m3d.name = readString(stream);
    auto meshSize{read<std::size_t>(stream)};

    for(auto i = 0u; i < meshSize; i++) {
        m3d.meshes.emplace_back(loadMesh(stream));
    }

    return m3d;
}

Opal::Resources::RMesh Opal::Resources::loadMesh(std::istream &stream) {
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

Opal::Resources::RTexture Opal::Resources::loadTexture(const std::string &name) {
    FIBITMAP *img;
    auto filename = "Resources/Textures/" + name + ".tga";
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    if(!FreeImage_FIFSupportsReading(format)) {
        throw std::runtime_error(filename + " couldn't be read by FreeImage");
    }

    if(format == FIF_UNKNOWN) {
        throw std::runtime_error(filename + " has unknown format");
    }

    img = FreeImage_Load(format, filename.c_str());

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

    std::vector<unsigned char> vb{bytes, bytes + width * height * RES_TEXTURE_BPP};
    FreeImage_Unload(img);
    return {std::move(vb), width, height};
}

Opal::Resources::RTexture Opal::Resources::loadTexture(std::istream &stream) {
    RTexture texture;
    auto size{read<std::size_t>(stream)};
    texture.name = readString(stream);
    texture.width = read<decltype(texture.width)>(stream);
    texture.height = read<decltype(texture.height)>(stream);
    texture.bytes.resize(texture.width * texture.height * RES_TEXTURE_BPP);
    stream.read((char*)texture.bytes.data(), texture.bytes.size());
    return texture;
}

Opal::Resources::RShader Opal::Resources::loadShader(const std::string &name) {
    RShader shader;

    return shader;
}

Opal::Resources::RShader Opal::Resources::loadShader(std::istream &stream) {
    RShader shader;

    return shader;
}
