#include "pixelpch.h"
#include "PositionComponent.h"
#include "ImGui/ImGuiLayer.h"

namespace Pixel {
    PositionComponent::PositionComponent(float x, float y, float z)
        : Component("Positional") {
        position = { x, y, z };
    }

    PositionComponent::PositionComponent() 
        : position({ 0, 0, 0 }), Component("Positional") { }

    void PositionComponent::Init() {

    }

    void PositionComponent::Draw() {

    }

    void PositionComponent::Update() {

    }

    void PositionComponent::ImGuiUpdate() {
        ImGui::Text("Positional Component");

        ImGui::DragFloat("X", &position.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("Y", &position.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("Z", &position.z, 0.1f, 0.0f, 0.0f, "%.2f");

    }
}