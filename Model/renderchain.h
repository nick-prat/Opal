#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include "renderobject.h"

class RenderChain
{
public:
    RenderChain();
	RenderChain(int num);
	RenderChain(int num, bool vol);
    ~RenderChain();
		
	bool InitRenderChain(int numObjects, bool vol);
	bool InitRenderChain(int numObjects);
	void Destroy();

    bool AttachRenderObject(RenderObject* object);
    void RenderObjectChain();

protected:

private:
	int m_objCount;
	int m_objLimit;
	bool m_volatile;
	RenderObject** m_memPool;
};

#endif // _RENDERCHAIN_H
