/*
 *  Resource Handler: functions for loading all supported data types
 *      - Loads JSON scene information from jsoncpp (https://github.com/open-source-parsers/jsoncpp)
 *      - Loads TGA textures via FreeImage
 *      - Loads 3Ds models via Assimp
 */

#include <Resources/resourcehandler.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <memory>
#include <vector>
#include <fstream>

#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Resources/texture.hpp>
#include <Resources/shader.hpp>
#include <Models/line.hpp>
#include <Models/staticmodel.hpp>

using json = nlohmann::json;

ResourceHandler::ResourceHandler() {

}

ResourceHandler::~ResourceHandler() {

}

void ResourceHandler::loadResources(const json& scene) {
    // TODO Implement texture loading
    // NOTE What other types of resources might i want to load on scene start
    if(scene.find("resources") != scene.end()) {
        std::vector<json> resources = scene["resources"];
        for(const json& resource : resources) {
            try {
                std::string type = resource["type"];
                std::string name = resource["resourcename"];

                if(type == "model3d") {
                    m_resources[name] = std::unique_ptr<const IResource>(loadModel3D(resource["filename"]));
                } else if(type == "shader") {
                    m_shaders[name] = std::unique_ptr<Shader>(loadShader(resource));
                }
            } catch(std::domain_error& error) {
                Log::getErrorLog() << error.what() << '\n';
            }
        }
    }
}

void ResourceHandler::addResource(const std::string& name, const IResource* const resource) {
    m_resources[name] = std::unique_ptr<const IResource>(resource);
}

IRenderObject* ResourceHandler::generateModel(const json& object) {
    std::string name = "";
    std::vector<glm::vec3> verts;

    std::vector<std::vector<float>> vertsf = object["vertices"];

    if(object.find("name") != object.end()) {
        name = object["name"];
    } else {
        name = "null";
    }

    for(std::vector<float> vert : vertsf) {
        if(vert.size() != 3) {
            throw BadResource("Vertex data size is not 3", name);
        }
        verts.push_back(glm::vec3(vert[0], vert[1], vert[2]));
    }

    std::vector<GLuint> indices;
    std::vector<unsigned int> indicesf = object["indices"];
    for(const unsigned int& index : indicesf) {
        if(index >= verts.size()) {
            throw BadResource("Index is out of range", name);
        }
        indices.push_back(index);
    }

    std::vector<glm::vec3> norms;
    if(object.find("noramls") != object.end()) {
        std::vector<std::vector<float>> normsf = object["normals"];
        for(const std::vector<float>& norm : normsf) {
            if(norm.size() != 3) {
                throw BadResource("Normal data size is not 3", name);
            }
            norms.push_back(glm::vec3(norm[0], norm[1], norm[2]));
        }
    } else {
        norms.clear();
        for(unsigned int i = 0; i < verts.size(); i++) {
            norms.push_back(glm::vec3(0.0f));
        }
    }

    std::vector<glm::vec2> uvs;
    if(object.find("uvs") != object.end()) {
        std::vector<std::vector<float>> uvsf = object["uvs"];
        for(const std::vector<float>& uv : uvsf) {
            if(uv.size() != 2) {
                throw BadResource("UV data size is not 2", name);
            }
            uvs.push_back(glm::vec2(uv[0], uv[1]));
        }
    } else {
        uvs.clear();
        for(unsigned int i = 0; i < verts.size(); i++) {
            uvs.push_back(glm::vec2(0.0f));
        }
    }

    std::vector<Model3D::Vertex> vertices;
    for(unsigned int i = 0; i < verts.size(); i++) {
        vertices.push_back(Model3D::Vertex(verts[i], norms[i], uvs[i]));
    }

    auto mesh = new Model3D::Mesh(vertices, indices);
    mesh->setMatName("texture");

    auto texture = loadTexture(object["matname"], true);

    auto model = new Model3D();
    model->addMesh(mesh);
    model->addTexture("texture", texture);

    return generateModel(object, model);
}

IRenderObject* ResourceHandler::generateModel(const json& object, const Model3D* const model3d) {
    std::string name = "";

    if(object.find("name") != object.end()) {
        name = object["name"];
    } else {
        name = "null";
    }

    if(model3d == nullptr) {
        throw BadResource("Model3D was null for ", name);
    }

    glm::mat4 transform = glm::mat4(1.0f);

    if(object.find("scale") != object.end()) {
        std::vector<float> scale = object["scale"];
        if(scale.size() != 3) {
            throw BadResource("Scale data size is not 3", name);
        }
        transform = glm::scale(transform, glm::vec3(scale[0], scale[1], scale[2]));
    }

    if(object.find("translation") != object.end()) {
        std::vector<float> translation = object["translation"];
        if(translation.size() != 3) {
            throw BadResource("Translation data size is not 3", name);
        }
        transform = glm::translate(transform, glm::vec3(translation[0], translation[1], translation[2]));
    }

    if(object.find("rotation") != object.end()) {
        std::vector<float> rotation = object["rotation"];
        if(rotation.size() != 3) {
            throw BadResource("Rotation data size is not 3", name);
        }

        float degrees = *(std::max_element(rotation.begin(), rotation.end()));
        if(degrees != 0.0f) {
            glm::vec3 amount = glm::vec3(rotation[0] / degrees, rotation[1] / degrees, rotation[2] / degrees);
            transform = glm::rotate(transform, degrees, amount);
        }
    }

    std::string shadername = object["shader"];
    auto shader = m_shaders.find(shadername);
    if(shader == m_shaders.end()) {
        throw BadResource("requested unknown shader " + shadername, name);
    }

    auto model = new StaticModel(model3d, transform);
    model->bindShader(shader->second.get());
    return model;
}

IRenderObject* ResourceHandler::generateLine(const json& object) {
    glm::vec3 head, tail, color;
    std::string name;

    if(object.find("name") != object.end()) {
        name = object["name"];
    } else {
        name = "null";
    }

    std::vector<float> head3f = object["head"];
    if(head3f.size() == 3) {
        head = glm::vec3(head3f[0], head3f[1], head3f[2]);
    } else {
        throw BadResource("head data size is not 3", name);
    }

    std::vector<float> tail3f = object["tail"];
    if(tail3f.size() == 3) {
        tail = glm::vec3(tail3f[0], tail3f[1], tail3f[2]);
    } else {
        throw BadResource("tail data size is not 3", name);
    }

    std::vector<float> color3f = object["color"];
    if(head3f.size() == 3) {
        color = glm::vec3(color3f[0], color3f[1], color3f[2]);
    } else {
        throw BadResource("color data size is not 3", name);
    }

    std::string shadername = object["shader"];
    auto shader = m_shaders.find(shadername);
    if(shader == m_shaders.end()) {
        throw BadResource("requested unknown shader " + shadername, name);
    }

    auto line = new Line(head, tail, color);
    line->bindShader(shader->second.get());
    return line;
}

Shader* ResourceHandler::loadShader(const json& object) {
    std::string name = object["resourcename"];
    std::string filename = object["filename"];
    std::vector<std::string> files = object["types"];
    if(files.size() == 0) {
        throw BadResource("types is empty", name);
    }

    std::vector<GLenum> types;
    for(auto& file : files) {
        if(file == "fragment") {
            types.push_back(GL_FRAGMENT_SHADER);
            file = filename + '/' + filename + "_fs.glsl";
        } else if(file == "vertex") {
            types.push_back(GL_VERTEX_SHADER);
            file = filename + '/' + filename + "_vs.glsl";
        }
    }

    auto shader = new Shader(files, types);

    if(object.find("uniforms") != object.end()) {
        std::vector<std::string> uniforms = object["uniforms"];
        for(const auto& uniform : uniforms) {
            shader->registerUniform(uniform);
        }
    }

    return shader;
}

const Texture* ResourceHandler::loadTexture(const std::string& name, bool genMipMaps) {
    auto resourcename = "tex_" + name;
    if(m_resources.find(resourcename) != m_resources.end()) {
        auto res = m_resources.find(resourcename);
        return getResource<Texture>(res->first);
    }

    FIBITMAP *img;
    auto filename = "Resources/Textures/" + name + ".tga";
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename.c_str());

    if(!FreeImage_FIFSupportsReading(format)) {
        throw BadResource("FreeImage can't read from file", filename);
    }

    if(format == FIF_UNKNOWN) {
        throw BadResource("Unknown format", filename);
    }

    img = FreeImage_Load(format, filename.c_str());

    if(!img) {
        throw BadResource("Couldn't load image data", filename);
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
        throw BadResource("couldn't load image bytes", filename);
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
        throw BadResource("texture is not valid", filename);
    }

    FreeImage_Unload(img);

    Texture* texture = new Texture();
    texture->setFileName(resourcename);
    texture->setTexture(glTexture);
    m_resources[resourcename] = std::unique_ptr<Texture>(texture);
    return texture;
}

const Model3D* ResourceHandler::loadModel3D(const std::string& modelname) {
    std::string filename = "Resources/Models/" + modelname + ".3ds";
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if(!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        throw BadResource(importer.GetErrorString(), filename);
    }

    auto model = new Model3D();

    aiMaterial** materials = scene->mMaterials;
    for(unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiString aName;
        materials[i]->Get(AI_MATKEY_NAME, aName);
        std::string name = std::string(aName.C_Str());

        try {
            auto temp = loadTexture(modelname + "/" + name, true);
            if(temp != nullptr) {
                model->addTexture(name, temp);
            }
        } catch (BadResource& error) {
            error.printError();
        }
    }

    std::vector<Model3D::Mesh*> meshes;
    try {
        loadNode(scene, scene->mRootNode, glm::mat4(1.0f), meshes);
    } catch(BadResource& error) {
        error.printError();
        delete model;
        for(const auto& mesh : meshes) {
            delete mesh;
        }
        throw BadResource("failed to load model nodes", modelname);
    }

    for(auto& mesh : meshes) {
        aiString aName;
        unsigned int index = mesh->getMatIndex();
        if(index < scene->mNumMaterials) {
            materials[mesh->getMatIndex()]->Get(AI_MATKEY_NAME, aName);
            mesh->setMatName(std::string(aName.C_Str()));
        }
        model->addMesh(mesh);
    }

    return model;
}

void ResourceHandler::copyaiMat(const aiMatrix4x4* from, glm::mat4& to) {
    to[0][0] = from->a1; to[1][0] = from->a2;
    to[2][0] = from->a3; to[3][0] = from->a4;
    to[0][1] = from->b1; to[1][1] = from->b2;
    to[2][1] = from->b3; to[3][1] = from->b4;
    to[0][2] = from->c1; to[1][2] = from->c2;
    to[2][2] = from->c3; to[3][2] = from->c4;
    to[0][3] = from->d1; to[1][3] = from->d2;
    to[2][3] = from->d3; to[3][3] = from->d4;
}

void ResourceHandler::loadNode(const aiScene* scene, const aiNode* node, glm::mat4 parentTransform, std::vector<Model3D::Mesh*>& meshes) {
    glm::mat4x4 transformation;
    copyaiMat(&node->mTransformation, transformation);
    transformation = parentTransform * transformation;

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        loadNode(scene, node->mChildren[i], transformation, meshes);
    }

    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::vector<Model3D::Vertex> vertices;

        for(unsigned int j = 0; j < mesh->mNumVertices; j++) {
            Model3D::Vertex vertex;

            glm::vec4 position = transformation * glm::vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f);
            vertex.position = glm::vec3(position.x, position.y, position.z);

            vertex.normal = (mesh->HasNormals())
                ? glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z)
                : glm::vec3(0.0f, 0.0f, 0.0f);

            vertex.texCoord = (mesh->HasTextureCoords(0))
                ? glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)
                : glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        std::vector<unsigned int> indices;
        if(mesh->HasFaces()) {
            for(unsigned int j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for(unsigned int k = 0; k < face.mNumIndices; k++) {
                    indices.push_back(face.mIndices[k]);
                }
            }
        } else {
            throw BadResource("Node was missing faces, load cancled");
        }

        auto rmesh = new Model3D::Mesh(vertices, indices);
        rmesh->setMatIndex(mesh->mMaterialIndex);
        meshes.push_back(rmesh);
    }
}

Shader* ResourceHandler::getShader(const std::string& shader) {
    return m_shaders[shader].get();
}
