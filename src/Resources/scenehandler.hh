#ifndef _RESOURCEHANDLER_H
#define _RESOURCEHANDLER_H

#include <string>
#include <fstream>
#include <unordered_map>

#include "resources.hh"

namespace Opal::Resources {

    class SceneHandler {
    public:
        SceneHandler() = default;

        void readFromJSON(std::ifstream &file);
        void readFromBIN(std::ifstream &file);
        void writeToJSON();
        void writeToBIN();

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
        std::string m_sceneName;
        std::unordered_map<std::string, RTexture> m_textures;
        std::unordered_map<std::string, RModel3D> m_model3ds;
    };

}

#endif // _RESOURCEHANDLER_H
