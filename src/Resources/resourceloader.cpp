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
#include <FreeImage.h>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <json.hpp>

#include <Utilities/utilities.hpp>
#include <Core/glapi.hpp>
#include <Resources/model3d.hpp>
#include <Models/line.hpp>
#include <Models/staticmodel.hpp>

using namespace gl;
using namespace Utilities;
using json = nlohmann::json;

std::shared_ptr<IRenderObject> LoadStaticModelJSON(json object) {
    std::string type = "";

    try {
        type = object["datatype"];
    } catch (std::domain_error& error) {
        type = "normal";
    }

    if(type == "normal") {
        return std::make_shared<StaticModel>(ResourceLoader::LoadModel3D(object["filename"]));
    } else if(type == "raw") {
        std::vector<glm::vec3> verts;
        std::vector<std::vector<float>> vertsf = object["vertices"];
        for(std::vector<float> vert : vertsf) {
            if(vert.size() != 3) {
                throw Exception("Vertex data is jumbled");
            }
            verts.push_back(glm::vec3(vert[0], vert[1], vert[2]));
        }

        std::vector<GLuint> indices;
        std::vector<uint> indicesf = object["indices"];
        for(uint index : indicesf) {
            if(index >= verts.size()) {
                throw Exception("Index is out of range");
            }
            indices.push_back(index);
        }

        std::vector<glm::vec3> norms;
        try {
            std::vector<std::vector<float>> normsf = object["normals"];
            for(std::vector<float> norm : normsf) {
                if(norm.size() != 3) {
                    throw Exception("Normal data is jumbled");
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
                    throw Exception("UV data is jumbled");
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
        return std::make_shared<StaticModel>(std::make_shared<Model3D>(meshes, textures));
    } else {
        throw Exception("Unknown data type");
    }
}

std::shared_ptr<IRenderObject> LoadLineJSON(json object) {
    glm::vec3 head, tail, color;

    std::vector<float> head3f = object["head"];
    if(head3f.size() == 3) {
        head = glm::vec3(head3f[0], head3f[1], head3f[2]);
    } else {
        throw Exception("head element size is incorrect");
    }

    std::vector<float> tail3f = object["tail"];
    if(tail3f.size() == 3) {
        tail = glm::vec3(tail3f[0], tail3f[1], tail3f[2]);
    } else {
        throw Exception("tail element size is incorrect");
    }

    std::vector<float> color3f = object["color"];
    if(head3f.size() == 3) {
        color = glm::vec3(color3f[0], color3f[1], color3f[2]);
    } else {
        throw Exception("color element size is incorrect");
        return nullptr;
    }

    return std::make_shared<Line>(head, tail, color);
}

std::vector<std::shared_ptr<IRenderObject>> ResourceLoader::LoadScene(std::string filename) {
    std::vector<std::shared_ptr<IRenderObject>> renderObjects;

    std::string contents;
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    } else {
        throw Exception("Couldn't load scene " + filename);
    }

    try {
        json scene = json::parse(contents);
        std::vector<json> objects = scene["staticObjects"];

        for(json object : objects) {
            std::string type = "";
            std::string name = "";

            try {
                try {
                    name = object["name"];
                } catch (std::domain_error& error) {
                    name = "null";
                }

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
            } catch (Exception& error) {
                std::cout << name << " : " << error.what() << std::endl;
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
        throw Exception("FreeImage can't read from file: " + filename);
    }

    if(format == FIF_UNKNOWN) {
        throw Exception("Unknown format for file: " + filename);
    }

    img = FreeImage_Load(format, filename.c_str());

    if(!img) {
        throw Exception("Couldn't load image data for file: " + filename);
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
        throw Exception("couldn't load image bytes for file: " + filename);
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
        throw Exception("texture is not valid for file: " + filename);
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
                ? glm::vec2(mesh->mTextureCoords[0][j].y, mesh->mTextureCoords[0][j].y)
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
            throw Exception("Node was missing faces, load cancled");
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
            } catch (Exception& error) {
                error.PrintError();
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
