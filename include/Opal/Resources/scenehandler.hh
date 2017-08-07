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
        SceneHandler(std::istream &stream);

        void writeToJSON(std::ostream &stream);
        void writeToBIN(std::ostream &stream);

        void deleteModel3D(const std::string &name);
        void deleteTexture(const std::string &name);
        void deleteShader(const std::string &name);

        bool addModel3D(const std::string &name, RModel3D &&model);
        bool addModel3D(std::tuple<std::string, RModel3D> &&model);
        bool addTexture(const std::string &name, RTexture &&texture);
        bool addTexture(std::tuple<std::string, RModel3D> &&texture);
        // void addShader(Shader &&shader);

        std::unordered_map<std::string, RTexture> &getTextures();
        std::unordered_map<std::string, RModel3D> &getModel3Ds();

        void info();
        void model3DInfo();
        void textureInfo();

    private:
        void readJSON(std::istream &stream);
        void readBIN(std::istream &stream);

    private:
        std::string m_sceneName;
        std::unordered_map<std::string, RTexture> m_textures;
        std::unordered_map<std::string, RModel3D> m_model3ds;
    };

}

#endif // _RESOURCEHANDLER_H
