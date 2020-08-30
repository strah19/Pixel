#ifndef vec2dD_H
#define vec2dD_H

#include <iostream>

namespace Pixel {
	class vec2d {
	public:
		float x;
		float y;

		vec2d(const float x, const float y);
		vec2d() :x(0), y(0) {}

		friend std::ostream& operator<<(std::ostream& os, const vec2d& vec2d);

		friend vec2d operator+(const vec2d& v1, const vec2d& v2);
		friend vec2d operator-(const vec2d& v1, const vec2d& v2);
		friend vec2d operator*(const vec2d& v1, const vec2d& v2);
		friend vec2d operator/(const vec2d& v1, const vec2d& v2);

		friend vec2d operator+(const vec2d& v1, const float s);
		friend vec2d operator-(const vec2d& v1, const float s);
		friend vec2d operator*(const vec2d& v1, const float s);
		friend vec2d operator/(const vec2d& v1, const float s);

		void operator+=(const float scalar);
		void operator-=(const float scalar);
		void operator*=(const float scalar);
		void operator/=(const float scalar);

		void operator+=(const vec2d& vec);
		void operator-=(const vec2d& vec);
		void operator*=(const vec2d& vec);
		void operator/=(const vec2d& vec);

		friend bool operator!=(const vec2d& v1, const vec2d& v2);
		friend bool operator==(const vec2d& v1, const vec2d& v2);

		void Negate();
		float Magnitude();
		vec2d Normalize();
		float DotProduct(vec2d& vec2d);
	};

	vec2d operator+(const vec2d& v1, const vec2d& v2);
	vec2d operator-(const vec2d& v1, const vec2d& v2);
	vec2d operator*(const vec2d& v1, const vec2d& v2);
	vec2d operator/(const vec2d& v1, const vec2d& v2);

	vec2d operator+(const vec2d& v1, const float s);
	vec2d operator-(const vec2d& v1, const float s);
	vec2d operator*(const vec2d& v1, const float s);
	vec2d operator/(const vec2d& v1, const float s);

	bool operator!=(const vec2d& v1, const vec2d& v2);
	bool operator==(const vec2d& v1, const vec2d& v2);
}

#endif // !vec2dD_H
