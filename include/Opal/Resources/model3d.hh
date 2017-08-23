#ifndef _MODEL3D_H
#define _MODEL3D_H

#include <Opal/Core/gl.hh>
#include <Opal/Resources/resources.hh>
#include <Opal/Resources/texture.hh>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Opal {

    class Model3D {
    private:
        struct Mesh {
            GLuint m_vbo, m_ibo;
            unsigned int m_indexCount;
            std::string m_matName;
        };

    public:
        Model3D(const Resources::RModel3D& model3d, std::unordered_map<std::string, Texture*>&& m_textures);
        Model3D(const Model3D&) = delete;
        Model3D(Model3D&& model);

        Model3D& operator=(Model3D&& model);

        const Texture& getTexture(const std::string& key) const;
        unsigned int getIndexCount(unsigned int i) const;
        std::string getMatName(unsigned int i) const;
        unsigned int getMeshCount() const;
        unsigned int getFaceCount() const;
        std::vector<GLuint> generateVAOs() const;
        void printTextures() const;

    private:
        std::array<glm::vec3, 2> m_boundingBox;
        std::vector<Mesh> m_meshes;
        // std::vector<GLuint> m_meshVBOs, m_meshIBOs;
        std::unordered_map<std::string, Texture*> m_textures;
    };

}

#endif // _MODEL3D_H
