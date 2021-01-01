#pragma once
#include <GLFW/glfw3.h>
#include <iomanip>

struct vec3 {
	GLfloat x, y, z;
	vec3() { x = 0; y = 0; z = 0; }
	vec3(GLfloat x, GLfloat y, GLfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	bool operator==(const vec3& other) const noexcept
	{
		return (other.x == this->x && other.y == this->y && other.z == this->z);
	}

	vec3 operator+(const vec3& other) const noexcept {
		return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	vec3 operator+(const glm::vec3& other) const noexcept {
		return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	vec3& operator+=(const vec3& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}
};

namespace std {

	template <>
	struct hash<vec3>
	{
		std::size_t operator()(const vec3& vect) const noexcept
		{
			std::hash<GLfloat> hasher;

			auto hash1 = hasher(vect.x);
			auto hash2 = hasher(vect.y);
			auto hash3 = hasher(vect.y);

			return std::hash<GLfloat>{}((hash1 ^ hash2 ^ hash3) >> 3);
		}
	};
}

struct vec2 {
	int x, y;
	vec2() { x = 0; y = 0; }
	vec2(int _x, int _y) {
		x = _x;
		y = _y;
	}

	bool operator==(const vec2& other) const noexcept
	{
		return (other.x == this->x && other.y == this->y);
	}

};

namespace std {

	template <>
	struct hash<vec2>
	{
		std::size_t operator()(const vec2& vect) const noexcept
		{
			std::hash<decltype(vect.x)> hasher;

			auto hash1 = hasher(vect.x);
			auto hash2 = hasher(vect.y);

			return std::hash<decltype(vect.x)>{}((hash1 ^ hash2) >> 2);
		}
	};
}


