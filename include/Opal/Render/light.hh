#ifndef _LIGHT_H
#define _LIGHT_H

#include <Opal/Core/gl.hh>
#include <glm/glm.hpp>

namespace Opal {

    class WorldLight {
    public:
        WorldLight();

        void setAmbientColor(glm::vec3 const& color);
        void setAmbientIntensity(float const intensity);
        glm::vec4 const& getAmbientColor() const;

        void setSunColor(glm::vec3 const& color);
        void setSunIntensity(float const intensity);
        void setSunDirection(glm::vec3 const& direction);
        glm::vec4 const& getSunColor() const;
        glm::vec4 const& getSunDirection() const;
        GLuint getSunUBO() const;

    private:
        void updateSunUBO();

        struct SunLight {
            glm::vec4 color;
            glm::vec4 direction;
        } m_sunLight;

    private:
        glm::vec4 m_ambientLight;
        GLuint m_sunUBO;
    };

}

#endif // _LIGHT_H
