#ifndef SCENE_H
#define SCENE_H

#include "ECS/EntityComponentSystem.h"

namespace Pixel {
	class Scene {
	public:
		~Scene();

		Entity* CreateEntity();
		void DestroyEntity(Entity* entity);

		inline std::vector<Entity*>& GetEntities() { return entities; }
	private:
		std::vector<Entity*> entities;
	};
}

#endif // !SCENE_H
