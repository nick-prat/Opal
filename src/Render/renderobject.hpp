#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <Render/Shader/shader.hpp>
#include <Display/display.hpp>

class IRenderObject
{
public:
    IRenderObject();
    virtual ~IRenderObject();
    virtual void Render() = 0;

    void Translate(const glm::vec3& trans);
    void Rotate(const float& degrees, const glm::vec3& rotate);
    void Scale(const glm::mat4& scale);

    glm::mat4 GetWorld();

protected:
    std::shared_ptr<GlutDisplay> m_display;

private:
    glm::mat4 m_translate;
    glm::mat4 m_scale;
    glm::mat4 m_rotate;
    glm::mat4 m_world;

};

#endif // _RENDEROBJECT_H
