#include <glm/glm.hpp>
#include <FreeImage.h>
#include <memory>
#include <vector>
#include <iostream>

#include "assimpmodel.hpp"
#include "assimploader.hpp"

AssimpModel::Texture LoadTexture(std::string filename)
{
    AssimpModel::Texture texture;
    FIBITMAP *img;
    filename = "./Textures/" + filename + ".tga";
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    img = FreeImage_Load(format, filename.c_str(), 0);

    if(!img)
    {
        std::cout << "Couldn't load image: " << filename << std::endl;
        return texture;
    }

    int height, width;
    FREE_IMAGE_TYPE type;
    BITMAPINFOHEADER* header;

    type = FreeImage_GetImageType(img);
    header = FreeImage_GetInfoHeader(img);
    width = FreeImage_GetWidth(img);
    height = FreeImage_GetHeight(img);

    std::cout << filename << " width: " << width << " height: " << height << std::endl;

    FreeImage_Unload(img);
    return texture;
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

    aiMaterial** materials = scene->mMaterials;
    std::unordered_map<std::string, AssimpModel::Texture> textures;
    for(uint i = 0; i < scene->mNumMaterials; i++)
    {
        aiString aName;
        materials[i]->Get(AI_MATKEY_NAME, aName);
        std::string name = std::string(aName.C_Str());

        if(textures.find(name) == textures.end())
        {
            textures[name] = LoadTexture(name);
        }
    }
    model->SetTextures(textures);

    std::vector<AssimpModel::AssimpMesh> meshes;

    for(uint i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
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
            return nullptr;
        }

        meshes.push_back(AssimpModel::AssimpMesh(vertices, indices));
    }

    model->SetMeshes(meshes);

    return model;
}
