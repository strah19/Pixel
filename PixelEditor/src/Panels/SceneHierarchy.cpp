#include "SceneHierarchy.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/Logger.h"

constexpr size_t MAX_INPUT_SIZE = 128;

namespace Pixel {
	SceneHierarchy::SceneHierarchy() : Layer("Scene Hiearchy") {
	}

	void SceneHierarchy::OnAttach() {
		scene = std::make_shared<Scene>();
	}

	void SceneHierarchy::OnDetach() {

	}

	void SceneHierarchy::UpdateGui() {
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			static char tag[MAX_INPUT_SIZE];
			ImGui::InputText("Entity Tag Name", tag, IM_ARRAYSIZE(tag));
			if (ImGui::Button("Create Entity")) {
				Entity* entity = scene->CreateEntity();
				entity->AddComponent<TagComponent>(tag);
				tag[0] = '\0';
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		auto& entity_list = scene->GetEntities();
		for (auto& entity : entity_list) {
			uint32_t flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entity->GetComponent<TagComponent>()->GetTagName().c_str());

			if (opened) {
				uint32_t pop_flags = ImGuiPopupFlags_MouseButtonRight;
				if (ImGui::BeginPopupContextItem(0, pop_flags)) {
					ImGui::Text("Add Component:");

					if (ImGui::Button("Positional Component")) {
						entity->AddComponent<PositionComponent>();
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}				
				
				if (ImGui::IsItemClicked()) 
					selected_entity = entity;

				for (auto& component : entity->GetComponents()) {
					component->Update();
					if (ImGui::TreeNodeEx((void*)(uint64_t)component->GetName().c_str(), flags, component->GetName().c_str()))
						ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}


		ImGui::End();

		ImGui::Begin("Properties");

		if (selected_entity != nullptr) {
			ImGui::Text(selected_entity->GetComponent<TagComponent>()->GetTagName().c_str());
			for (auto& component : selected_entity->GetComponents()) {
				ImGui::Separator();
				component->ImGuiUpdate();
			}

		}

		ImGui::End();
	}

	void SceneHierarchy::OnUpdate(float delta) {

	}

	void SceneHierarchy::UserDefEvent(Event& event) {

	}
}