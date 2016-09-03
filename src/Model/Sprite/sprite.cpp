#include <iostream>

#include "sprite.hpp"

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

bool Sprite::InitSprite(int width, int height, std::string texture)
{
    return true;
}

void Sprite::Destroy()
{

}

bool Sprite::AttachShader(Shader* shader)
{
    return true;
}

void Sprite::Render()
{
	std::cout << "Render!" << std::endl;
}
