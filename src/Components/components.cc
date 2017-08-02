#include <Opal/Components/components.hh>

#include <iostream>
#include <glm/gtx/transform.hpp>

using namespace Opal;

CBody::CBody(const glm::vec3 &loc)
: m_rotate(glm::mat4(1.0f))
, m_location(glm::translate(loc)) {}

void CBody::setLocation(const glm::vec3 &loc) {
    m_location = glm::translate(loc);
}

void CBody::setLocation(const glm::mat4 &loc) {
    m_location = loc;
}

const glm::mat4 &CBody::getLocation() const {
    return m_location;
}

void CBody::setRotation(const glm::vec3 &rotation) {
    float max = glm::max(glm::max(rotation.x, rotation.y), rotation.z);
    m_rotate = glm::rotate(m_rotate, max, rotation / max);
}

void CBody::setRotation(const glm::mat4 &rotation) {
    m_rotate = rotation;
}

const glm::mat4 &CBody::getRotation() const {
    return m_rotate;
}

void CBody::setScale(const glm::vec3 &scale) {
    m_scale = glm::scale(scale);
}

void CBody::setScale(const glm::mat4 &scale) {
    m_scale = scale;
}

const glm::mat4 &CBody::getScale() const {
    return m_scale;
}

// CPhysics

CPhysics::CPhysics(const glm::vec3 &dir)
: m_direction(dir) {}

void CPhysics::setDirection(const glm::vec3 &dir) {
    m_direction = dir;
}

glm::vec3 CPhysics::getDirection() const {
    return m_direction;
}

// CRender

CRender::CRender(const Model3D &model)
: m_model(model)
, m_vaos(m_model.generateVAOs()) {}

void CRender::bind(unsigned int i) const {
    glBindVertexArray(m_vaos[i]);
}

unsigned int CRender::getIndexCount(unsigned int i) const {
    return m_model.getIndexCount(i);
}

const std::vector<GLuint> &CRender::getVAOs() const {
    return m_vaos;
}

const Model3D &CRender::getModel() const {
    return m_model;
}
