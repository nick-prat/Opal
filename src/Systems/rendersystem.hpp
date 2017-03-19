#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include <vector>
#include <string>
#include <list>

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Utilities/log.hpp>
#include <Render/renderobject.hpp>
#include <ECS/systems.hpp>
#include <ECS/components.hpp>

// NOTE What else are shader's capable of? Do i need to implement more functions in this wrapper?
// TODO Implement lighting in shaders
class Shader {
public:
    Shader(std::vector<std::string>& fileNames, const std::vector<GLenum>& types);

    void attachRenderObject(IRenderObject* object);
    void detachRenderObject(IRenderObject* object);

    void registerUniform(const std::string& name);
    GLint getUniformLocation(const std::string& name) const;

    GLuint getProgram() const;
    std::size_t getRenderCount() const;

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;
    std::list<IRenderObject*> m_renderObjects;
    std::unordered_map<std::string, GLint> m_uniformLocations;
};

template<typename entity_manager_t>
class RenderSystem : public ISystem<RenderSystem<entity_manager_t>, entity_manager_t> {
public:
    static_assert(entity_manager_t::template contains<CRender>(), "RenderSystem requires invalid type CRender");
    static_assert(entity_manager_t::template contains<CLocation>(), "RenderSystem requires invalid type CLocation");

    RenderSystem(entity_manager_t* entityManager)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(entityManager)
    , m_ambientColor(glm::vec4(1.0f)) {}

    RenderSystem(const RenderSystem&) = delete;

    RenderSystem(RenderSystem&& renderSystem)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(renderSystem.m_entityManager)
    , m_ambientColor(renderSystem.m_ambientColor)
    , m_shader(renderSystem.m_shader) {
        renderSystem.m_shader = nullptr;
    }

    RenderSystem& operator=(const RenderSystem&) = delete;

    RenderSystem& operator=(RenderSystem&& renderSystem) {
        m_ambientColor = renderSystem.m_ambientColor;
        m_shader = renderSystem.m_shader;

        renderSystem.m_ambientColor = glm::vec4(0.0f);
        renderSystem.m_shader = nullptr;

        return *this;
    }

    void update() {
        glUseProgram(m_shader->getProgram());

        for(auto& entity : this->m_entities) {
            if(entity->template hasComponent<CRender>()) {
                std::cout << "yolo\n";
            }
            //auto& rend = entity->template getComponent<CRender>();
            //auto& loc = entity->template getComponent<CLocation>();
        }
    }

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    void setShader(Shader* shader) {
        m_shader = shader;
    }

    std::size_t getRenderCount() const {
        return m_shader->getRenderCount();
    }

    void setAmbientColor(const glm::vec3& color) {
        m_ambientColor.r = color.r;
        m_ambientColor.g = color.g;
        m_ambientColor.b = color.b;
    }

    void setAmbientIntensity(const float intensity) {
        m_ambientColor.a = intensity;
    }

private:
    const Display* m_display;
    glm::vec4 m_ambientColor;
    Shader* m_shader;
};

#endif // _RENDER_SYSTEM_H
