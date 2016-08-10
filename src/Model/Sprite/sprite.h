#ifndef _SPRITE_H
#define _SPRITE_H

#include <string>

#include "Model/renderobject.h"
#include "Model/Shader/shader.h"

class Sprite : public RenderObject
{
    public:
        Sprite();
        ~Sprite();

        bool InitSprite(int width, int height, std::string texture);
        void Destroy();

        bool AttachShader(Shader* shader);
        void Render();

    protected:

    private:
};

#endif // _SPRITE_H
