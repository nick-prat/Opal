#include "renderobject.hpp"

int IRenderObject::m_numRenderObjects = 0;

IRenderObject::IRenderObject()
        : m_tag(m_numRenderObjects) {
    m_numRenderObjects++;
}

IRenderObject::~IRenderObject() {

}

int IRenderObject::getNumRenderObjects() {
    return m_numRenderObjects;
}

int IRenderObject::getTag() {
    return m_tag;
}
