#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>

class WorldLight {
public:
    void setAmbientColor(const glm::vec3& color);
    void setAmbientIntensity(const float intensity);
    const glm::vec4& getAmbientColor() const;

    void setSunColor(const glm::vec3& color);
    void setSunIntensity(const float intensity);
    void setSunDirection(const glm::vec3& direction);
    const glm::vec4& getSunColor() const;
    const glm::vec3& getSunDirection() const;

private:
    glm::vec4 m_ambientLight;
    glm::vec4 m_sunColor;
    glm::vec3 m_sunDirection;

};

#endif // _LIGHT_H
