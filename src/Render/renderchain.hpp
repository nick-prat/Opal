#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <list>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <GL/gl3w.h>

class Display;
class IRenderObject;
class Shader;

class RenderChain {
public:
    RenderChain();

    void attachShader(Shader* shader);
    void detachShader(Shader* shader);

    void render(const Display* const display) const;
    void clearShaders();
    int getRenderCount() const;

    void setAmbientColor(const glm::vec3& color);
    void setAmbientIntensity(const float intensity);

private:
    glm::vec4 m_ambientColor;
    std::unordered_set<Shader*> m_shaders;
};

// NOTE What else are shader's capable of? Do i need to implement more functions in this wrapper?
// TODO Implement lighting in shaders
class Shader {
    friend class RenderChain;
public:
    Shader(std::vector<std::string>& fileNames, const std::vector<GLenum>& types);

    void attachRenderObject(IRenderObject* object);
    void detachRenderObject(IRenderObject* object);

    void registerUniform(const std::string& name);
    GLint getUniformLocation(const std::string& name) const;

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;
    std::list<IRenderObject*> m_renderObjects;
    std::unordered_map<std::string, GLint> m_uniformLocations;
};

#endif // _RENDERCHAIN_H
