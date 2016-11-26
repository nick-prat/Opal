#include "scene.hpp"

#include <iostream>

Scene::Scene() {

}

Scene::~Scene() {

}

Entity* Scene::GetEntity(const std::string& name) const {
    if(m_entities.find(name) != m_entities.end()) {
        return (*m_entities.find(name)).second.get();
    }
    return nullptr;
}

void Scene::AddEntity(const std::string& name, Entity* ent) {
    m_entities[name] = std::unique_ptr<Entity>(std::move(ent));
}
