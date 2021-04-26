#ifndef INSTANCE_RENDERER_H
#define INSTANCE_RENDERER_H

#include "Models/Model.h"

namespace Pixel {
	class InstanceRenderer {
	public:
		static void Init();
		static void Destroy();

		static void BeginScene(Camera& camera);
		static void EndScene();
		static void NewBatch();

		static void GoToNextDrawCommand();
		static void MakeCommand();

		static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color);
	private:
		static void StartBatch();
		static void Render();

		static float CalculateTextureIndex(std::shared_ptr<Texture>& texture);
	};
}

#endif // !INSTANCE_RENDERER_H
