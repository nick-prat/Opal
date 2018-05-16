#include <Opal/Render/light.hh>

#include <cstring>

Opal::WorldLight::WorldLight() {
    m_sunLight.color = glm::vec4{1.0f, 1.0f, 1.0f, 10.0f};
    m_sunLight.direction = glm::vec4{10.0f, 10.0f, 10.0f, 0.0f};

    glGenBuffers(1, &m_sunUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_sunUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SunLight), &m_sunLight, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Opal::WorldLight::setAmbientColor(glm::vec3 const& color) {
    m_ambientLight.r = color.r;
    m_ambientLight.g = color.g;
    m_ambientLight.b = color.b;
}

void Opal::WorldLight::setAmbientIntensity(float const intensity) {
    m_ambientLight.a = intensity;
}

glm::vec4 const& Opal::WorldLight::getAmbientColor() const {
    return m_ambientLight;
}

void Opal::WorldLight::setSunColor(glm::vec3 const& color) {
    m_sunLight.color.r = color.r;
    m_sunLight.color.g = color.g;
    m_sunLight.color.b = color.b;
    updateSunUBO();
}

void Opal::WorldLight::setSunIntensity(float const intensity) {
    m_sunLight.color.a = intensity;
    updateSunUBO();
}

void Opal::WorldLight::setSunDirection(glm::vec3 const& direction) {
    m_sunLight.direction.x = direction.x;
    m_sunLight.direction.y = direction.y;
    m_sunLight.direction.z = direction.z;
    updateSunUBO();
}

glm::vec4 const& Opal::WorldLight::getSunColor() const {
    return m_sunLight.color;
}

glm::vec4 const& Opal::WorldLight::getSunDirection() const {
    return m_sunLight.direction;
}

void Opal::WorldLight::updateSunUBO() {
    glBindBuffer(GL_UNIFORM_BUFFER, m_sunUBO);

    GLvoid* buff = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(SunLight), GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);
    memcpy(buff, &m_sunLight, sizeof(SunLight));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

GLuint Opal::WorldLight::getSunUBO() const {
    return m_sunUBO;
}
