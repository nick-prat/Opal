#include "light.hh"

#include <cstring>

Opal::WorldLight::WorldLight() {
    m_sunLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f);
    m_sunLight.location = glm::vec4(10.0f, 10.0f, 10.0f, 0.0f);

    glGenBuffers(1, &m_sunUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_sunUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SunLight), &m_sunLight, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Opal::WorldLight::setAmbientColor(const glm::vec3 &color) {
    m_ambientLight.r = color.r;
    m_ambientLight.g = color.g;
    m_ambientLight.b = color.b;
}

void Opal::WorldLight::setAmbientIntensity(const float intensity) {
    m_ambientLight.a = intensity;
}

const glm::vec4 &Opal::WorldLight::getAmbientColor() const {
    return m_ambientLight;
}

void Opal::WorldLight::setSunColor(const glm::vec3 &color) {
    m_sunLight.color.r = color.r;
    m_sunLight.color.g = color.g;
    m_sunLight.color.b = color.b;
    updateSunUBO();
}

void Opal::WorldLight::setSunIntensity(const float intensity) {
    m_sunLight.color.a = intensity;
    updateSunUBO();
}

void Opal::WorldLight::setSunLocation(const glm::vec3 &location) {
    m_sunLight.location.x = location.x;
    m_sunLight.location.y = location.y;
    m_sunLight.location.z = location.z;
    updateSunUBO();
}

const glm::vec4 &Opal::WorldLight::getSunColor() const {
    return m_sunLight.color;
}

const glm::vec4 &Opal::WorldLight::getSunLocation() const {
    return m_sunLight.location;
}

void Opal::WorldLight::updateSunUBO() {
    glBindBuffer(GL_UNIFORM_BUFFER, m_sunUBO);

    GLvoid* buff = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(buff, &m_sunLight, sizeof(SunLight));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

GLuint Opal::WorldLight::getSunUBO() const {
    return m_sunUBO;
}
