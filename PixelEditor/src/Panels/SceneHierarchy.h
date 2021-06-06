#ifndef SCENE_HIERARCHY_H
#define SCENE_HIERARCHY_H

#include "Core/Layer.h"
#include "ECS/Scene/Scene.h"
#include "ECS/Component.h"

namespace Pixel {
	class SceneHierarchy : public Layer {
	public:
		SceneHierarchy();

		void OnAttach();
		void OnDetach();
		void UpdateGui();

		void OnUpdate(float delta);
		void UserDefEvent(Event& event);
	private:
		std::shared_ptr<Scene> scene;
		Entity* selected_entity = nullptr;
	};
}

#endif // !SCENE_HIERARCHY_H
