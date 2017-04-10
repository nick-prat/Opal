#ifndef _SYSTEMS_H
#define _SYSTEMS_H

#include <ECS/entity.hpp>
#include <ECS/components.hpp>

#include <iostream>
#include <unordered_set>

class IBaseSystem {
public:
    typedef int system_id;
    inline static system_id systemIdCounter = 0;

    virtual ~IBaseSystem() {}
    virtual void update() = 0;
};

template<typename system_t>
class ISystem : public IBaseSystem {
public:
    static system_id getSystemID() {
        static system_id systemId = systemIdCounter++;
        return systemId;
    }

    ISystem() = default;
    virtual ~ISystem() {}

    ISystem(const ISystem&) = delete;

    ISystem(ISystem<system_t>&& system)
    : m_entities(std::move(system.m_entities)) {}

    ISystem& operator=(const ISystem&) = delete;
    ISystem& operator=(ISystem&& system) = delete;

    void subscribe(unsigned int entityID) {
        m_entities.insert(entityID);
    }

    template<typename entity_manager_t>
    void update(entity_manager_t& entMan) {
        static_cast<system_t*>(this)->update(entMan);
    }

protected:
    inline static system_id systemId;
    std::unordered_set<unsigned int> m_entities;
};

#endif // _SYSTEMS_H
