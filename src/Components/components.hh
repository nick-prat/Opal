#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/gl3w.h>

#include <Resources/texture.hh>
#include <Resources/model3d.hh>

class CLocation {
public:
    CLocation() = default;
    CLocation(const glm::vec3& loc);

    void setLocation(const glm::vec3& loc);
    void setLocation(const glm::mat4& loc);
    const glm::mat4& getLocation() const;

    void setDirection(const glm::vec3& dir);
    glm::vec3 getDirection() const;

private:
    glm::mat4 m_location;
    glm::vec3 m_direction;
};

class CRender {
public:
    CRender() = default;
    CRender(const Model3D& model);

    const glm::mat4& getRotation() const;
    void setRotation(const glm::mat4& rotation);

    const glm::mat4& getScale() const;
    void setScale(const glm::mat4& scale);

    const std::vector<GLuint> getVAOs() const;
    const Model3D& getModel() const;

private:
    const Model3D& m_model;
    std::vector<GLuint> m_vaos;
    glm::mat4 m_rotate;
    glm::mat4 m_scale;
};

#endif // _COMPONENTS_H
