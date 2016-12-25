#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <memory>
#include <list>

#include <Core/display.hpp>
#include <Render/renderobject.hpp>

class RenderChain {
public:
    RenderChain();
    ~RenderChain();

    void attach(IRenderObject* object);
    void detach(IRenderObject* object);
    void render(const Display* const display) const;
    void clear();
    int getRenderCount() const;

private:
    std::list<IRenderObject*> m_objects;
};

#endif // _RENDERCHAIN_H
