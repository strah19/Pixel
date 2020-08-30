#include "pixelpch.h"
#include "vec2d.h"

namespace Pixel {
	vec2d::vec2d(const float x, const float y)
		: x(x), y(y) {
	}

	std::ostream& operator<<(std::ostream& os, const vec2d& vec2d) {
		os << vec2d.x << " " << vec2d.y;
		return os;
	}

	vec2d operator+(const vec2d& v1, const vec2d& v2) {
		return (vec2d(v1.x + v2.x, v1.y + v2.y));
	}

	vec2d operator-(const vec2d& v1, const vec2d& v2) {
		return (vec2d(v1.x - v2.x, v1.y - v2.y));
	}

	vec2d operator*(const vec2d& v1, const vec2d& v2) {
		return (vec2d(v1.x * v2.x, v1.y * v2.y));
	}

	vec2d operator/(const vec2d& v1, const vec2d& v2) {
		return (vec2d(v1.x / v2.x, v1.y / v2.y));
	}

	vec2d operator+(const vec2d& v1, const float s) {
		return (vec2d(v1.x + s, v1.y + s));
	}

	vec2d operator-(const vec2d& v1, const float s) {
		return (vec2d(v1.x - s, v1.y - s));
	}

	vec2d operator*(const vec2d& v1, const float s) {
		return (vec2d(v1.x * s, v1.y * s));
	}

	vec2d operator/(const vec2d& v1, const float s) {
		return (vec2d(v1.x / s, v1.y / s));
	}

	void vec2d::operator+=(const float scalar) {
		this->x += scalar;
		this->y += scalar;
	}

	void vec2d::operator-=(const float scalar) {
		this->x -= scalar;
		this->y -= scalar;
	}

	void vec2d::operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

	void vec2d::operator/=(const float scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}

	void vec2d::operator+=(const vec2d& vec) {
		this->x += vec.x;
		this->y += vec.y;
	}

	void vec2d::operator-=(const vec2d& vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void vec2d::operator*=(const vec2d& vec) {
		this->x *= vec.x;
		this->y *= vec.y;
	}

	void vec2d::operator/=(const vec2d& vec) {
		this->x /= vec.x;
		this->y /= vec.y;
	}

	void vec2d::Negate() {
		this->x *= -1;
		this->y *= -1;
	}

	float vec2d::Magnitude() {
		return (std::sqrt((this->x * this->x) + (this->y * this->y)));
	}

	vec2d vec2d::Normalize() {
		vec2d return_vec2d = { this->x, this->y };
		float m = Magnitude();
		return_vec2d.x /= m;
		return_vec2d.y /= m;

		return return_vec2d;
	}

	bool operator!=(const vec2d& v1, const vec2d& v2) {
		if (v1.x != v2.x && v1.y != v2.y) {
			return true;
		}
		return false;
	}

	bool operator==(const vec2d& v1, const vec2d& v2) {
		if (v1.x == v2.x && v1.y == v2.y) {
			return true;
		}
		return false;
	}

	float vec2d::DotProduct(vec2d& vec2d) {
		return ((this->x * vec2d.x) + (this->y * vec2d.y));
	}

}