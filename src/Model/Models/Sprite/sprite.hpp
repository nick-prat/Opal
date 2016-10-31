#ifndef _SPRITE_H
#define _SPRITE_H

#include <string>

#include <Model/Render/renderobject.hpp>
#include <Model/Shader/shader.hpp>

class Sprite : public IRenderObject
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
