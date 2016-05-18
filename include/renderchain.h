#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include "renderobject.h"

class RenderChain
{
    public:
        RenderChain();
        ~RenderChain();

        void AttachRenderObject(RenderObject* object);
        void RenderObjectChain();

    protected:

    private:
        struct RenderObjectContainer
        {
            RenderObjectContainer();
            ~RenderObjectContainer();

            RenderObjectContainer* next;
            RenderObject* object;
        };

        RenderObjectContainer* m_renderQueue;
};

#endif // _RENDERCHAIN_H
