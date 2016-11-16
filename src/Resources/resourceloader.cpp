/*
 *
 *  Resource loader, functions for loading all supported data types
 *      - Loads scene information from json
 *      - Loads textures form TGA's
 *      - Loads models from 3ds
 *
 *  TODO Change all functions to throw errors instead of return values
 *
 */

#include "resourceloader.hpp"

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <FreeImage.h>
#include <json.hpp>

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

#include <Utilities/exceptions.hpp>
#include <Core/glapi.hpp>
#include <Resources/model3d.hpp>
#include <Models/line.hpp>
#include <Models/staticmodel.hpp>

using namespace gl;
using json = nlohmann::json;

std::shared_ptr<IRenderObject> LoadStaticModelJSON(json object) {
    std::string type = "";
    std::string name = "";

    try {
        type = object["datatype"];
    } catch (std::domain_error& error) {
        type = "normal";
    }

    try {
        name = object["name"];
    } catch (std::domain_error& error) {
        name = "null";
    }

    std::shared_ptr<IRenderObject> rObject;

    if(type == "normal") {
        rObject = std::make_shared<StaticModel>(ResourceLoader::LoadModel3D(object["filename"]));
    } else if(type == "raw") {
        std::vector<glm::vec3> verts;
        std::vector<std::vector<float>> vertsf = object["vertices"];
        for(std::vector<float> vert : vertsf) {
            if(vert.size() != 3) {
                throw bad_resource("Vertex data size is not 3", name);
            }
            verts.push_back(glm::vec3(vert[0], vert[1], vert[2]));
        }

        std::vector<GLuint> indices;
        std::vector<uint> indicesf = object["indices"];
        for(uint index : indicesf) {
            if(index >= verts.size()) {
                throw bad_resource("Index is out of range", name);
            }
            indices.push_back(index);
        }

        std::vector<glm::vec3> norms;
        try {
            std::vector<std::vector<float>> normsf = object["normals"];
            for(std::vector<float> norm : normsf) {
                if(norm.size() != 3) {
                    throw bad_resource("Normal data size is not 3", name);
                }
                norms.push_back(glm::vec3(norm[0], norm[1], norm[2]));
            }
        } catch (std::domain_error& error) {
            norms.clear();
            for(uint i = 0; i < verts.size(); i++) {
                norms.push_back(glm::vec3(0.0f));
            }
        }

        std::vector<glm::vec2> uvs;
        try {
            std::vector<std::vector<float>> uvsf = object["uvs"];
            for(std::vector<float> uv : uvsf) {
                if(uv.size() != 2) {
                    throw bad_resource("UV data size is not 2", name);
                }
                uvs.push_back(glm::vec2(uv[0], uv[1]));
            }
        } catch (std::domain_error& error) {
            uvs.clear();
            for(uint i = 0; i < verts.size(); i++) {
                uvs.push_back(glm::vec2(0.0f));
            }
        }

        std::vector<Model3D::Vertex> vertices;
        for(uint i = 0; i < verts.size(); i++) {
            vertices.push_back(Model3D::Vertex(verts[i], norms[i], uvs[i]));
        }

        std::vector<std::shared_ptr<Model3D::Mesh>> meshes;
        meshes.push_back(std::make_shared<Model3D::Mesh>(vertices, indices));
        meshes[0]->SetMatName("texture");

        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        textures["texture"] = ResourceLoader::LoadTexture(object["matname"], true);

        rObject = std::make_shared<StaticModel>(std::make_shared<Model3D>(meshes, textures));
    } else {
        throw bad_resource("Unknown data type", name);
    }

    try {
        std::vector<float> scale = object["scale"];
        if(scale.size() != 3) {
            throw std::domain_error("scale data size is not 3");
        }
        rObject->Scale(glm::vec3(scale[0], scale[1], scale[2]));
    } catch (std::domain_error& error) {}

    try {
        std::vector<float> translation = object["translation"];
        if(translation.size() != 3) {
            throw bad_resource("translation data size is not 3", name);
        }
        rObject->Translate(glm::vec3(translation[0], translation[1], translation[2]));
    } catch (std::domain_error& error) {}

    try {
        std::vector<float> rotation = object["rotation"];
        if(rotation.size() != 3) {
            throw bad_resource("Rotation data size is not 3", name);
        }
    } catch (std::domain_error& error) {}

    return rObject;
}

std::shared_ptr<IRenderObject> LoadLineJSON(json object) {
    glm::vec3 head, tail, color;
    std::string name;

    try {
        name = object["name"];
    } catch (std::domain_error& error) {
        name = "null";
    }

    std::vector<float> head3f = object["head"];
    if(head3f.size() == 3) {
        head = glm::vec3(head3f[0], head3f[1], head3f[2]);
    } else {
        throw bad_resource("head data size is not 3", name);
    }

    std::vector<float> tail3f = object["tail"];
    if(tail3f.size() == 3) {
        tail = glm::vec3(tail3f[0], tail3f[1], tail3f[2]);
    } else {
        throw bad_resource("tail data size is not 3", name);
    }

    std::vector<float> color3f = object["color"];
    if(head3f.size() == 3) {
        color = glm::vec3(color3f[0], color3f[1], color3f[2]);
    } else {
        throw bad_resource("color data size is not 3", name);
    }

    return std::make_shared<Line>(head, tail, color);
}

std::vector<std::shared_ptr<IRenderObject>> ResourceLoader::LoadScene(std::string filename) {
    std::vector<std::shared_ptr<IRenderObject>> renderObjects;

    filename = "Scenes/" + filename + ".json";
    std::string contents;
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    } else {
        throw generic_exception(filename + " doesn't exist");
    }

    try {
        json scene = json::parse(contents);
        std::vector<json> objects = scene["staticObjects"];

        for(json object : objects) {
            std::string type = "";
            std::string name = "";

            try {
                type = object["type"];

                std::shared_ptr<IRenderObject> rObject;
                if(type == "line") {
                    rObject = LoadLineJSON(object);
                } else if(type == "staticmodel") {
                    rObject = LoadStaticModelJSON(object);
                }
                if(rObject != nullptr) {
                    renderObjects.push_back(rObject);
                }
            } catch (bad_resource& error) {
                std::cout << "[" << error.GetResourceName() << "] " << error.GetError() << std::endl;
            }
        }
    } catch(std::exception& error) {
        std::cout << "Parsing of " << filename << " failed : " << error.what() << std::endl;
    }

    return renderObjects;
}

std::shared_ptr<Texture> ResourceLoader::LoadTexture(std::string filename, bool genMipMaps) {
    FIBITMAP *img;
    filename = "./Textures/" + filename + ".tga";
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    if(!FreeImage_FIFSupportsReading(format)) {
        throw bad_resource("FreeImage can't read from file", filename);
    }

    if(format == FIF_UNKNOWN) {
        throw bad_resource("Unknown format", filename);
    }

    img = FreeImage_Load(format, filename.c_str());

    if(!img) {
        throw bad_resource("Couldn't load image data", filename);
    }

    if(FreeImage_GetBPP(img) != 32) {
        FIBITMAP* oldImg = img;
        img = FreeImage_ConvertTo32Bits(oldImg);
        FreeImage_Unload(oldImg);
    }

    int height, width;
    width = FreeImage_GetWidth(img);
    height = FreeImage_GetHeight(img);

    unsigned char* bytes = FreeImage_GetBits(img);

    if(bytes == nullptr) {
        FreeImage_Unload(img);
        throw bad_resource("couldn't load image bytes", filename);
    }

    GLuint glTexture;
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);

    if(genMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    if(!glIsTexture(glTexture)) {
        FreeImage_Unload(img);
        throw bad_resource("texture is not valid", filename);
    }

    FreeImage_Unload(img);

    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->SetFileName(filename);
    texture->SetTexture(glTexture);
    return texture;
}

void CopyaiMat(const aiMatrix4x4* from, glm::mat4& to) {
    to[0][0] = from->a1; to[1][0] = from->a2;
    to[2][0] = from->a3; to[3][0] = from->a4;
    to[0][1] = from->b1; to[1][1] = from->b2;
    to[2][1] = from->b3; to[3][1] = from->b4;
    to[0][2] = from->c1; to[1][2] = from->c2;
    to[2][2] = from->c3; to[3][2] = from->c4;
    to[0][3] = from->d1; to[1][3] = from->d2;
    to[2][3] = from->d3; to[3][3] = from->d4;
}

void LoadNode(const aiScene* scene, const aiNode* node, std::vector<std::shared_ptr<Model3D::Mesh>>& meshes) {
    for(uint i = 0; i < node->mNumChildren; i++) {
        LoadNode(scene, node->mChildren[i], meshes);
    }

    for(uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<Model3D::Vertex> vertices;

        for(uint j = 0; j < mesh->mNumVertices; j++) {
            Model3D::Vertex vertex;

            vertex.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

            vertex.normal = (mesh->HasNormals())
                ? glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z)
                : glm::vec3(0.0f, 0.0f, 0.0f);

            vertex.texCoord = (mesh->HasTextureCoords(0))
                ? glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
                : glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        std::vector<uint> indices;
        if(mesh->HasFaces()) {
            for(uint j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for(uint k = 0; k < face.mNumIndices; k++) {
                    indices.push_back(face.mIndices[k]);
                }
            }
        } else {
            throw bad_resource("Node was missing faces, load cancled");
        }

        glm::mat4x4 transformation;
        CopyaiMat(&node->mTransformation, transformation);
        std::shared_ptr<Model3D::Mesh> rmesh = std::make_shared<Model3D::Mesh>(vertices, indices);
        rmesh->SetTransformation(transformation);
        rmesh->SetMatIndex(mesh->mMaterialIndex);

        meshes.push_back(rmesh);
    }
}

std::shared_ptr<Model3D> ResourceLoader::LoadModel3D(std::string modelname) {
    std::string filename = "Models/" + modelname + ".3ds";
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if(!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        std::cout << "Couldn't open model: " << filename << std::endl;
        std::cout << "Assimp: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    auto model = std::make_shared<Model3D>();

    aiMaterial** materials = scene->mMaterials;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    for(uint i = 0; i < scene->mNumMaterials; i++) {
        aiString aName;
        materials[i]->Get(AI_MATKEY_NAME, aName);
        std::string name = std::string(aName.C_Str());

        if(textures.find(name) == textures.end()) {
            try {
                std::shared_ptr<Texture> temp = LoadTexture(modelname + "/" + name, true);
                if(temp != nullptr) {
                    textures[name] = temp;
                }
            } catch (bad_resource& error) {
                std::cout << "[" << error.GetResourceName() << "] " << error.GetError() << std::endl;
            }
        }
    }
    model->SetTextures(textures);

    std::vector<std::shared_ptr<Model3D::Mesh>> meshes;
    LoadNode(scene, scene->mRootNode, meshes);

    for(std::shared_ptr<Model3D::Mesh>& mesh : meshes) {
        aiString aName;
        uint index = mesh->GetMatIndex();
        if(index < scene->mNumMaterials) {
            materials[mesh->GetMatIndex()]->Get(AI_MATKEY_NAME, aName);
            mesh->SetMatName(std::string(aName.C_Str()));
        }
    }

    model->SetMeshes(meshes);

    return model;
}
