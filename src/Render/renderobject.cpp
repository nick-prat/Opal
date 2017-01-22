#include "renderobject.hpp"

#include <Render/renderchain.hpp>

int IRenderObject::m_numRenderObjects = 0;

IRenderObject::IRenderObject()
        : m_shader(nullptr) {
    m_numRenderObjects++;
}

IRenderObject::~IRenderObject() {
    m_numRenderObjects--;
}

int IRenderObject::getNumRenderObjects() {
    return m_numRenderObjects;
}

void IRenderObject::bindShader(Shader* shader) {
    m_shader = shader;
}

Shader* IRenderObject::getShader() {
    return m_shader;
}
