#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>
#include <Core/gl.hh>

namespace Opal {

    class WorldLight {
    public:
        WorldLight();

        void setAmbientColor(const glm::vec3 &color);
        void setAmbientIntensity(const float intensity);
        const glm::vec4 &getAmbientColor() const;

        void setSunColor(const glm::vec3 &color);
        void setSunIntensity(const float intensity);
        void setSunLocation(const glm::vec3 &direction);
        const glm::vec4 &getSunColor() const;
        const glm::vec4 &getSunLocation() const;
        GLuint getSunUBO() const;

    private:
        void updateSunUBO();

        struct SunLight {
            glm::vec4 color;
            glm::vec4 location;
        } m_sunLight;

    private:
        glm::vec4 m_ambientLight;
        GLuint m_sunUBO;
    };

}

#endif // _LIGHT_H
