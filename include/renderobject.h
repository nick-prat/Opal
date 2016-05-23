#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include "shader.h"

class RenderObject
{
public:
	RenderObject();
	virtual ~RenderObject();
	virtual void Render() = 0;
	virtual bool InitObject() = 0;
	virtual void Destroy() = 0;

protected:

private:	
	
};

#endif // _RENDEROBJECT_H
