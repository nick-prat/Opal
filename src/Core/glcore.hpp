#ifndef _GLCORE_H
#define _GLCORE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/display.hpp>
#include <Render/renderchain.hpp>
#include <Models/staticmodel.hpp>
#include <Models/line.hpp>

class GLCore {
public:
    GLCore(int width, int height, std::string scene);
    ~GLCore();

    void DisplayFunc();
    void KeyboardFunc(int key, bool state);

    void LoadScene(std::string name);

private:
    std::vector<std::shared_ptr<IRenderObject>> m_renderObjects;
    std::shared_ptr<Display> m_display;
    std::vector<std::shared_ptr<Line>> m_lines;
    std::vector<std::shared_ptr<StaticModel>> m_staticModels;
};

#endif // _GLCORE_H
