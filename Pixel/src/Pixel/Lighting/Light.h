#ifndef LIGHT_H
#define LIGHT_H

#include<glm.hpp>
#include "Renderer/Shader.h"

namespace Pixel {
	struct LightSource {
		glm::vec3 position;
		glm::vec3 view_pos;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		LightSource(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position, const glm::vec3& view_pos) 
			: ambient(ambient), diffuse(diffuse), specular(specular), position(position), view_pos(view_pos) { }

		LightSource(const LightSource& light_source) {
			ambient = light_source.ambient;
			diffuse = light_source.diffuse;
			specular = light_source.specular;

			position = light_source.position;
			view_pos = light_source.view_pos;
		}

		LightSource() = default;

		bool operator==(const LightSource& source) {
			return (position == source.position && view_pos == source.view_pos && ambient == source.ambient && diffuse == source.diffuse && specular == source.specular);
		}

		void PassToShader(std::shared_ptr<Shader>* shader);
	};
}

#endif // !LIGHT_H
