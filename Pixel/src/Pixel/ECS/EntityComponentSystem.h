#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <cctype> // For size_t
#include <vector> // For vector
#include <bitset> // For bitset
#include <memory> // For shared_ptr

using ComponentID = size_t;

constexpr size_t MAX_COMPONENTS = 32;

inline ComponentID GetComponentTypeID() {
    static ComponentID current_component_id = 0;
    return current_component_id++;
}

template <typename T> inline ComponentID GetComponentTypeID() {
    static ComponentID type_id = GetComponentTypeID();
    return type_id;
}

class Entity;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void Init() { }
    virtual void Draw() { }
    virtual void Update() { }

    Entity* GetEntity() { return parent; }
    void SetEntity(Entity* parent) { this->parent = parent; }
protected:
    Entity* parent;
};

class Entity {
public:
    Entity();

    template <typename T, typename ... Ts>
    inline T& AddComponent(Ts&&... ts){
        T* component(new T(std::forward<Ts>(ts)...));
        component->SetEntity(this);

        std::shared_ptr<Component> component_pointer{ component };
        components.emplace_back(std::move(component_pointer));

        component_set[GetComponentTypeID<T>()] = true;

        component->Init();
        return *component;
    }

    template <typename T>
    inline bool HasComponent() {
        return component_set[GetComponentTypeID<T>];
    }

    template <typename T>
    inline T* GetComponent() {
        auto component_pointer = components[GetComponentTypeID<T>()];
        return static_cast<T*>(component_pointer.get());
    }
private:
    std::vector<std::shared_ptr<Component>> components;
    std::bitset<MAX_COMPONENTS> component_set;
};

#endif //!ENTITY_COMPONENT_SYSTEM_H