#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "glm.hpp"
#include "Renderer/Texture.h"

namespace Pixel {
	constexpr size_t SPRITE_COORD_SIZE = 4;
	
	class Spritesheet {
	public:
		void Init(std::shared_ptr<Texture>& texture);
		int CalculateSpriteCoordinate(const glm::vec2& pixel_point);
		
		glm::vec2* FindRectSprite(const glm::vec2& pixel_point, const glm::vec2& sprite_size);

		inline glm::vec2 GetCoordinate() const { return in_range_coordinates; }
	private:
		std::shared_ptr<Texture> texture;
		glm::vec2 in_range_coordinates;

		glm::vec2 rect_coordinates[SPRITE_COORD_SIZE];
	};
}

#endif // !SPRITE_SHEET_H
