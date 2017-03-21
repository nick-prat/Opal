#include "light.hpp"

void WorldLight::setAmbientColor(const glm::vec3 &color) {
    m_ambientLight.r = color.r;
    m_ambientLight.g = color.g;
    m_ambientLight.b = color.b;
}

void WorldLight::setAmbientIntensity(const float intensity) {
    m_ambientLight.a = intensity;
}

const glm::vec4& WorldLight::getAmbientColor() const {
    return m_ambientLight;
}

void WorldLight::setSunColor(const glm::vec3 &color) {
    m_sunColor.r = color.r;
    m_sunColor.g = color.g;
    m_sunColor.b = color.b;
}

void WorldLight::setSunIntensity(const float intensity) {
    m_sunColor.a = intensity;
}

void WorldLight::setSunDirection(const glm::vec3 &direction) {
    m_sunDirection = direction;
}

const glm::vec4& WorldLight::getSunColor() const {
    return m_sunColor;
}

const glm::vec3& WorldLight::getSunDirection() const {
    return m_sunDirection;
}
