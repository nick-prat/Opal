#include "renderobject.hpp"

int IRenderObject::m_tagCounter = 0;
int IRenderObject::m_numRenderObjects = 0;

IRenderObject::IRenderObject()
        : m_tag(m_numRenderObjects) {
    m_tagCounter++;
    m_numRenderObjects++;
}

IRenderObject::~IRenderObject() {
    m_numRenderObjects--;
}

int IRenderObject::getNumRenderObjects() {
    return m_numRenderObjects;
}

int IRenderObject::getTag() {
    return m_tag;
}
