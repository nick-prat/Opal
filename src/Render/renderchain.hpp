#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <list>
#include <glm/glm.hpp>
#include <unordered_map>

class Display;
class IRenderObject;
class Shader;

class RenderChain {
public:
    RenderChain();
    ~RenderChain();

    void attach(IRenderObject* object);
    void detach(IRenderObject* object);
    void render(const Display* const display) const;
    void clear();
    int getRenderCount() const;

    void setAmbientColor(const glm::vec3& color);
    void setAmbientIntensity(const float intensity);

private:
    glm::vec4 m_ambientColor;
    std::unordered_map<Shader*, std::list<IRenderObject*>> m_objects;
};

#endif // _RENDERCHAIN_H
