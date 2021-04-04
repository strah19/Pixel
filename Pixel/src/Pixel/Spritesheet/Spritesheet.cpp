#include "pixelpch.h"
#include "Spritesheet.h"

namespace Pixel {

	void Spritesheet::Init(std::shared_ptr<Texture>& texture) {
		this->texture = texture;
	}

	int Spritesheet::CalculateSpriteCoordinate(const glm::vec2& pixel_point) {
		if (!texture)
			return -1;
		if (pixel_point.x > texture->GetWidth() || pixel_point.y > texture->GetHeight())
			return -1;

		in_range_coordinates = pixel_point / glm::vec2(texture->GetWidth(), texture->GetHeight());

		return 0;
	}

	glm::vec2* Spritesheet::CalculateRectCoordinates(const glm::vec2& pixel_point, const glm::vec2& sprite_size) {
		float y_coord = texture->GetHeight() - (pixel_point.y + sprite_size.y);

		glm::vec2 pixel_coordinates[] = {
			{ pixel_point.x, y_coord },
			{ pixel_point.x + sprite_size.x, y_coord},
			{ pixel_point.x + sprite_size.x, y_coord + sprite_size.y },
			{ pixel_point.x, y_coord + sprite_size.y }
		};

		for (size_t i = 0; i < SPRITE_COORD_SIZE; i++) {
			if (CalculateSpriteCoordinate(pixel_coordinates[i]) > -1) {
				rect_coordinates[i] = GetCoordinate();
			}
		}

		return rect_coordinates;
	}

	glm::vec2* Spritesheet::FetchRectCoordinatesWithDivision(const glm::vec2& pixel_point) {
		return CalculateRectCoordinates(pixel_point, sprite_size);
	}
}