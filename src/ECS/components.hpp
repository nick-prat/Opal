#ifndef _COMPONENTS_H
#define _COMPONENTS_H

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
};

#endif // _COMPONENTS_H
