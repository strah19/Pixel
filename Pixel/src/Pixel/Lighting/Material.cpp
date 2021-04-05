#include "pixelpch.h"
#include "Material.h"
#include "Renderer/Renderer.h"

namespace Pixel {
	void TextureMaterial::PassToShader(std::shared_ptr<Shader>* shader, uint32_t& latest_texture_index) {
		if (initialized) {
			diffuse->get()->Bind(latest_texture_index);
			shader->get()->Set1f("material.diffuse", latest_texture_index);
			latest_texture_index++;

			if (latest_texture_index == MAX_TEXTURE_SLOTS) 
				Pixel::Renderer::NewBatch();

			specular->get()->Bind(latest_texture_index);
			shader->get()->Set1f("material.specular", latest_texture_index);

			shader->get()->Set1f("material.shininess", shininess);
			light_source.PassToShader(shader);
		}
	}

	void BasicMaterial::PassToShader(std::shared_ptr<Shader>* shader, uint32_t& latest_texture_index) {
		if (initialized) {
			shader->get()->SetVec3f("material.ambient", ambient);
			shader->get()->SetVec3f("material.specular", specular);
			shader->get()->SetVec3f("material.diffuse", diffuse);

			shader->get()->Set1f("material.shininess", shininess);

			light_source.PassToShader(shader);
		}
	}
}