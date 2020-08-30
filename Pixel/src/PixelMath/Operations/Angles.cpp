#include "pixelpch.h"
#include "Angles.h"
#include "Symbols.h"

namespace Pixel {
	float DegreeToRadian(float degree) {
		return (degree * (PI / 180));
	}

	float RadianToDegree(float radian) {
		return (radian * (180 / PI));
	}
}