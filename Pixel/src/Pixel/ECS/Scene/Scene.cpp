#include "pixelpch.h"
#include "Scene.h"

namespace Pixel {
	Scene::~Scene() {
		for (auto& entity : entities)
			delete entity;
	}

	Entity* Scene::CreateEntity() {
		entities.push_back(new Entity());
		return entities.back();
	}

	void Scene::DestroyEntity(Entity* entity) {
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it != entities.end()) {
			entities.erase(it);
		}
	}
}