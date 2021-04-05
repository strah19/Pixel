#include "pixelpch.h"
#include "Light.h"
#include "Renderer/Renderer.h"

namespace Pixel {
	void LightSource::PassToShader(std::shared_ptr<Shader>* shader) {
		shader->get()->SetVec3f("light.position", position);
		shader->get()->SetVec3f("viewPos", view_pos);

		shader->get()->SetVec3f("light.ambient", ambient);
		shader->get()->SetVec3f("light.diffuse", diffuse);
		shader->get()->SetVec3f("light.specular", specular);
	}
}