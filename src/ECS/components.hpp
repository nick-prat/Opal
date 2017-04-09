#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/gl3w.h>

#include <iostream>

class CLocation {
public:
    CLocation() = default;
    CLocation(const glm::vec3& loc);

    void setLocation(const glm::vec3& loc);
    glm::vec3 getLocation() const;

    void setDirection(const glm::vec3& dir);
    glm::vec3 getDirection() const;

private:
    glm::vec3 m_location;
    glm::vec3 m_direction;
};

class CRender {
public:
    CRender() = default;
    CRender(std::vector<GLuint>&& vaos);

    glm::mat4 getRotation() const;
    void setRotation(const glm::mat4& rotation);

    glm::mat4 getScale() const;
    void setScale(const glm::mat4& scale);

    const std::vector<GLuint> getVAOs() const;

private:
    std::vector<GLuint> m_vaos;
    glm::mat4 m_rotate;
    glm::mat4 m_scale;
};

#endif // _COMPONENTS_H
