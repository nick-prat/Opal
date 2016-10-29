#include "resourceloader.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <FreeImage.h>
#include <memory>
#include <vector>
#include <iostream>

#include <Utilities/utilities.hpp>
#include <Core/glapi.hpp>
#include <Resources/model3d.hpp>
#include <Utilities/utilities.hpp>

using namespace gl;

std::shared_ptr<Texture> ResourceLoader::LoadTexture(std::string filename, bool genMipMaps)
{
    // TODO Change to freeimageplus at some point

    FIBITMAP *img;
    filename = "./Textures/" + filename + ".tga";
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    if(!FreeImage_FIFSupportsReading(format))
    {
        std::cout << "FreeImage can't read from this file" << std::endl;
        return nullptr;
    }

    if(format == FIF_UNKNOWN)
    {
        std::cout << "Unknown format: " << filename << std::endl;
        return nullptr;
    }

    img = FreeImage_Load(format, filename.c_str(), 0);

    if(!img)
    {
        std::cout << "Couldn't load image: " << filename << std::endl;
        return nullptr;
    }

    if(FreeImage_GetBPP(img) != 32)
    {
        std::cout << "converting to 32 bits for image " << filename << std::endl;
        FIBITMAP* oldImg = img;
        //img = FreeImage_ConvertTo24Bits(oldImg);
        img = FreeImage_ConvertTo32Bits(oldImg);
        FreeImage_Unload(oldImg);
    }

    int height, width;
    width = FreeImage_GetWidth(img);
    height = FreeImage_GetHeight(img);

    unsigned char* bytes = FreeImage_GetBits(img);

    if(bytes == nullptr)
    {
        std::cout << "couldn't load image bytes for " << filename << std::endl;
    }

    GLuint glTexture;
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);

    if(genMipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    if(!glIsTexture(glTexture))
    {
        std::cout << "texture is not valid " << glTexture << std::endl;
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

bool LoadNode(const aiScene* scene, const aiNode* node, std::vector<std::shared_ptr<Model3D::Mesh>>& meshes)
{
    for(uint i = 0; i < node->mNumChildren; i++)
    {
        if(!LoadNode(scene, node->mChildren[i], meshes))
        {
            return false;
        }
    }

    for(uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<Model3D::Vertex> vertices;

        for(uint j = 0; j < mesh->mNumVertices; j++)
        {
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
        if(mesh->HasFaces())
        {
            for(uint j = 0; j < mesh->mNumFaces; j++)
            {
                aiFace face = mesh->mFaces[j];
                for(uint k = 0; k < face.mNumIndices; k++)
                {
                    indices.push_back(face.mIndices[k]);
                }
            }
        }
        else
        {
            std::cout << "Node was missing faces, load cancled" << std::endl;
            return false;
        }

        glm::mat4x4 transformation;
        CopyaiMat(&node->mTransformation, transformation);
        std::shared_ptr<Model3D::Mesh> rmesh = std::make_shared<Model3D::Mesh>(vertices, indices);
        rmesh->SetTransformation(transformation);
        rmesh->SetMatIndex(mesh->mMaterialIndex);

        meshes.push_back(rmesh);
    }

    return true;
}

std::shared_ptr<Model3D> ResourceLoader::LoadModel3D(std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if(!scene)
    {
        std::cout << "Couldn't open model: " << filename << std::endl;
        return nullptr;
    }

    auto model = std::make_shared<Model3D>();

    aiMaterial** materials = scene->mMaterials;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    for(uint i = 0; i < scene->mNumMaterials; i++)
    {
        aiString aName;
        materials[i]->Get(AI_MATKEY_NAME, aName);
        std::string name = std::string(aName.C_Str());

        if(textures.find(name) == textures.end())
        {
            std::shared_ptr<Texture> temp = LoadTexture(name, true);
            if(temp != nullptr)
            {
                textures[name] = temp;
            }
        }
    }
    model->SetTextures(textures);

    std::vector<std::shared_ptr<Model3D::Mesh>> meshes;
    LoadNode(scene, scene->mRootNode, meshes);

    for(std::shared_ptr<Model3D::Mesh>& mesh : meshes)
    {
        aiString aName;
        uint index = mesh->GetMatIndex();
        if(index < scene->mNumMaterials)
        {
            materials[mesh->GetMatIndex()]->Get(AI_MATKEY_NAME, aName);
            mesh->SetMatName(std::string(aName.C_Str()));
        }
    }

    model->SetMeshes(meshes);

    return model;
}
