#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <FreeImage.h>
#include <memory>
#include <vector>
#include <iostream>

#include <Utilities/utilities.hpp>
#include <Core/glapi.hpp>
#include <Model/Textures/texture.hpp>

#include "assimpmodel.hpp"
#include "assimploader.hpp"

bool LoadTexture(Texture& texture, std::string filename)
{
    FIBITMAP *img;
    filename = "./Textures/" + filename + ".tga";
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    texture.SetFileName(filename);

    if(format == FIF_UNKNOWN)
    {
        std::cout << "Unknown format: " << filename << std::endl;
        return false;
    }

    img = FreeImage_Load(format, filename.c_str(), 0);

    if(!img)
    {
        std::cout << "Couldn't load image: " << filename << std::endl;
        return false;
    }

    if(FreeImage_GetBPP(img) != 32)
    {
        FIBITMAP* oldImg = img;
        img = FreeImage_ConvertTo32Bits(oldImg);
        FreeImage_Unload(oldImg);
    }

    int height, width;
    width = FreeImage_GetWidth(img);
    height = FreeImage_GetHeight(img);

    // TODO Load image and return it

    unsigned char* bytes = FreeImage_GetBits(img);

    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    FreeImage_Unload(img);

    texture.SetTexture(glTexture);

    return true;
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

void LoadNode(const aiScene* scene, const aiNode* node, std::vector<AssimpModel::AssimpMesh>& meshes)
{
    for(uint i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(scene, node->mChildren[i], meshes);
    }

    for(uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<AssimpModel::Vertex> vertices;

        for(uint j = 0; j < mesh->mNumVertices; j++)
        {
            AssimpModel::Vertex vertex;

            vertex.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
            vertex.normal = (mesh->HasNormals()) ? glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z) : glm::vec3(0.0f, 0.0f, 0.0f);

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
            return;
        }

        glm::mat4x4 transformation;
        CopyaiMat(&node->mTransformation, transformation);
        std::cout << glm::to_string(transformation) << std::endl;
        AssimpModel::AssimpMesh rmesh(vertices, indices);
        rmesh.SetTransformation(transformation);

        meshes.push_back(rmesh);
    }
}

std::shared_ptr<AssimpModel> AssimpLoader::LoadModel(std::string filename)
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

    std::cout << "Loading model: " << filename << std::endl;

    auto model = std::make_shared<AssimpModel>();

    std::vector<AssimpModel::AssimpMesh> meshes;
    LoadNode(scene, scene->mRootNode, meshes);
    model->SetMeshes(meshes);

    aiMaterial** materials = scene->mMaterials;
    std::unordered_map<std::string, Texture> textures;
    for(uint i = 0; i < scene->mNumMaterials; i++)
    {
        aiString aName;
        materials[i]->Get(AI_MATKEY_NAME, aName);
        std::string name = std::string(aName.C_Str());

        if(textures.find(name) == textures.end())
        {
            LoadTexture(textures[name], name);
        }
    }
    model->SetTextures(textures);

    return model;
}
