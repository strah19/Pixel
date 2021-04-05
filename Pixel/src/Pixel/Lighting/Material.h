#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdint.h>
#include <glm.hpp>
#include "Renderer/Texture.h"
#include "Light.h"

namespace Pixel {
	struct Material {
		bool initialized = false;

		Material() = default;

		virtual void PassToShader(std::shared_ptr<Shader>* shader, uint32_t& latest_texture_index) = 0;
	};

	struct TextureMaterial : public Material{
		std::shared_ptr<Texture>* diffuse;
		std::shared_ptr<Texture>* specular;
		float shininess;
		LightSource light_source;

		TextureMaterial(std::shared_ptr<Texture>& diffuse, std::shared_ptr<Texture>& specular, float shininess, const LightSource& light_source)
			: diffuse(&diffuse), specular(&specular), shininess(shininess), light_source(light_source) {
			initialized = true;
		}

		TextureMaterial(const TextureMaterial& material) {
			diffuse = material.diffuse;
			specular = material.specular;
			shininess = material.shininess;
			light_source = material.light_source;

			initialized = true;
		}

		TextureMaterial() = default;

		void PassToShader(std::shared_ptr<Shader>* shader, uint32_t& latest_texture_index) override;
	};

	struct BasicMaterial : public Material{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		LightSource light_source;

		float shininess;

		BasicMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const LightSource& light_source)
			: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), light_source(light_source) {
			initialized = true;
		}

		BasicMaterial(const BasicMaterial& material) {
			ambient = material.ambient;
			diffuse = material.diffuse;
			specular = material.specular;

			shininess = material.shininess;
			light_source = material.light_source;

			initialized = true;
		}

		BasicMaterial() = default;

		void PassToShader(std::shared_ptr<Shader>* shader, uint32_t& latest_texture_index) override;
	};
}

#endif // !MATERIAL_H
