#include "renderobject.hpp"

int IRenderObject::m_numRenderObjects = 0;

IRenderObject::IRenderObject()
        : m_tag(m_numRenderObjects) {
    m_numRenderObjects++;
}

IRenderObject::~IRenderObject() {

}

int IRenderObject::GetNumRenderObjects() {
    return m_numRenderObjects;
}

int IRenderObject::GetTag() {
    return m_tag;
}
