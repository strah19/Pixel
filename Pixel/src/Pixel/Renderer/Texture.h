#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

namespace Pixel {
	class Texture {
	public:
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void UnBind() = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetTextureId() const = 0;

		static std::shared_ptr<Texture> CreateTexture(const char* file_path);
		static std::shared_ptr<Texture> CreateTexture(uint32_t width, uint32_t height);
	};
}

#endif // !TEXTURE_H
