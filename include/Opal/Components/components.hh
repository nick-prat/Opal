#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <Opal/Core/gl.hh>
#include <Opal/Resources/model3d.hh>

#include <vector>
#include <glm/glm.hpp>

namespace Opal {

    class CBody {
    public:
        CBody() = default;
        CBody(const glm::vec3& loc);

        void setLocation(const glm::vec3& loc);
        void setLocation(const glm::mat4& loc);
        const glm::mat4& getLocation() const;

        void setRotation(const glm::vec3& rotation);
        void setRotation(const glm::mat4& rotation);
        const glm::mat4& getRotation() const;

        void setScale(const glm::vec3& scale);
        void setScale(const glm::mat4& scale);
        const glm::mat4& getScale() const;


    private:
        glm::mat4 m_rotate;
        glm::mat4 m_scale;
        glm::mat4 m_location;
    };

    class CPhysics {
    public:
        CPhysics() = default;
        CPhysics(const glm::vec3& dir);

        void setDirection(const glm::vec3& dir);
        glm::vec3 getDirection() const;

    private:
        glm::vec3 m_direction;
    };

    class CRender {
    public:
        CRender(const Model3D& model);

        void bind(unsigned int i) const;
        unsigned int getIndexCount(unsigned int i) const;
        const std::vector<GLuint>& getVAOs() const;
        const Model3D& getModel() const;

    private:
        const Model3D& m_model;
        std::vector<GLuint> m_vaos;
    };

    struct CTerrain {
        GLuint vao;
        unsigned int indexCount;
        glm::vec3 origin;
    };

}

#endif // _COMPONENTS_H
