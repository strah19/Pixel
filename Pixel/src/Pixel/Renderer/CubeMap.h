#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <memory>
#include <vector>
#include <string>
#include "glm.hpp"

namespace Pixel {
	constexpr glm::vec3 SKYBOX_VERTICES[] = {
	{-1.0f,  1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f},
	{ 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f},
	{-1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f},
	{-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
	};

	class CubeMap {
	public:
		virtual uint32_t GetTexture() = 0;
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void UnBind() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;

		static std::shared_ptr<CubeMap> CreateCubeMap(const std::vector<std::string>& faces);
	};
}

#endif // !CUBE_MAP_H
