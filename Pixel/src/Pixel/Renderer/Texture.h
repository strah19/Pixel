#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

namespace Pixel {
	class Texture {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<Texture> CreateTexture(const char* file_path);
	};
}

#endif // !TEXTURE_H
