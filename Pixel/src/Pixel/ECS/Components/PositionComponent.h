#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "ECS/EntityComponentSystem.h"
#include <glm.hpp>

namespace Pixel {
    class PositionComponent : public Component {
    public:
        PositionComponent(float x, float y, float z);
        PositionComponent();
        virtual ~PositionComponent() = default;

        virtual void Init() override;
        virtual void Draw() override;
        virtual void Update() override;
        virtual void ImGuiUpdate() override;

        inline void Position(const glm::vec3& position) { this->position = position; }
        inline glm::vec3 Position() const { return position; }
    private:
        glm::vec3 position = { 0, 0, 0 };
    };
}

#endif //!POSITION_COMPONENT_H