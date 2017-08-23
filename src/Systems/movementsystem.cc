#include <Opal/Systems/movementsystem.hh>

void Opal::MovementSystem::update(Emerald::EntityManager& entMan) {
    entMan.mapEntities<CBody, CPhysics>([&entMan](auto ent) {
        auto& loc = entMan.getComponent<CBody>(ent);
        auto& phys = entMan.getComponent<CPhysics>(ent);
        loc.setLocation(glm::translate(loc.getLocation(), phys.getDirection()));
    });
}
