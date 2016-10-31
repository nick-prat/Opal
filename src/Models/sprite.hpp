#ifndef _SPRITE_H
#define _SPRITE_H

#include <string>

#include <Render/renderobject.hpp>
#include <Render/Shader/shader.hpp>

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
