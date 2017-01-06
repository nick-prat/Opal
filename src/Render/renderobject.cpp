#include "renderobject.hpp"

#include <Resources/shader.hpp>

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

Shader* IRenderObject::getShader() {
    return m_shader.get();
}
