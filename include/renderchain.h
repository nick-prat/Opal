#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include "renderobject.h"

class RenderChain
{
    public:
        RenderChain();
        ~RenderChain();
		
		bool InitRenderChain(int numObjects);
		void Destroy();

        bool AttachRenderObject(RenderObject* object);
        void RenderObjectChain();

    protected:

    private:
		int m_objCount;
		int m_objLimit;
		RenderObject** m_memPool;
};

#endif // _RENDERCHAIN_H
