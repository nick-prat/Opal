#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <memory>

#include <Model/Assimp/assimpmodel.h>
#include <Model/renderobject.h>

class StaticModel : public IRenderObject
{
    StaticModel(std::shared_ptr<AssimpModel> model);
    ~StaticModel();

    void Render();
};

#endif // _STATIC_MODEL_H
