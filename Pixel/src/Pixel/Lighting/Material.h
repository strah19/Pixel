#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdint.h>
#include <glm.hpp>
#include "Renderer/Texture.h"
#include "Light.h"

namespace Pixel {
	struct TextureMaterial {
		std::shared_ptr<Texture>* diffuse;
		std::shared_ptr<Texture>* specular;
		float shininess;
		LightSource light_source;

		TextureMaterial(std::shared_ptr<Texture>& diffuse, std::shared_ptr<Texture>& specular, float shininess, const LightSource& light_source)
			: diffuse(&diffuse), specular(&specular), shininess(shininess), light_source(light_source) {
		}

		TextureMaterial(const TextureMaterial& material) {
			diffuse = material.diffuse;
			specular = material.specular;
			shininess = material.shininess;
			light_source = material.light_source;
		}

		TextureMaterial() = default;
	};

	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float shininess;

		Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
			: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
		}

		Material(const Material& material) {
			ambient = material.ambient;
			diffuse = material.diffuse;
			specular = material.specular;

			shininess = material.shininess;
		}

		Material() = default;
	};
}

#endif // !MATERIAL_H
