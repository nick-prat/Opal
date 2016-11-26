#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>

#include <Entity/entity.hpp>

class Scene {
public:
    Scene();
    ~Scene();

    Entity* GetEntity(const std::string& name) const;
    void AddEntity(const std::string& name, Entity* ent);

private:
    std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
};

#endif // _SCENE_H
