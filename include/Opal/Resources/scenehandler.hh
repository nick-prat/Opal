#ifndef _RESOURCEHANDLER_H
#define _RESOURCEHANDLER_H

#include <string>
#include <fstream>
#include <unordered_map>

#include <Opal/Resources/resources.hh>

namespace Opal::Resources {

    class SceneHandler {
    public:
        SceneHandler() = default;
        SceneHandler(const std::string& scene);
        SceneHandler(std::istream& stream);

        SceneHandler& operator=(SceneHandler&& scene) noexcept;

        void read(std::istream& stream);
        void readJSON(std::istream& stream);
        void readBIN(std::istream& stream);

        void writeToJSON(std::ostream& stream);
        void writeToBIN(std::ostream& stream);

        void deleteModel3D(const std::string& name);
        void deleteTexture(const std::string& name);
        void deleteShader(const std::string& name);

        bool addModel3D(RModel3D&& model);
        bool addModel3D(std::tuple<std::string, RModel3D>&& model);
        bool addTexture(RTexture&& texture);
        bool addTexture(std::tuple<std::string, RModel3D>&& texture);
        bool addShader(RShader&& shader);

        std::unordered_map<std::string, RTexture>& getTextures();
        std::unordered_map<std::string, RModel3D>& getModel3Ds();
        std::unordered_map<std::string, RShader>& getShaders();

        void info();
        void model3DInfo();
        void textureInfo();

        std::string getSceneName();

    private:
        std::string m_sceneName;
        std::unordered_map<std::string, RTexture> m_textures;
        std::unordered_map<std::string, RModel3D> m_model3ds;
        std::unordered_map<std::string, RShader> m_shaders;
    };

}

#endif // _RESOURCEHANDLER_H
