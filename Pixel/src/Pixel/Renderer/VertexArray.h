#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <vector>

namespace Pixel {
	enum class VertexShaderType {
		None, Float, Int
	};

	struct VertexBufferElement {
		uint32_t size;
		uint32_t index;
		VertexShaderType type;
		bool normalized;
		uint32_t offset;

		VertexBufferElement(uint32_t size, bool normalized, VertexShaderType type)
			: size(size), index(0), normalized(normalized), type(type), offset(0) { }
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
			: elements(elements) {
			Calculate();
		}	

		VertexBufferLayout() { }

		void AddToBuffer(const VertexBufferElement& element) {
			elements.push_back(element);
			elements.back().index = (uint32_t) (elements.size() - 1);
		}

		uint32_t Calculate() {
			uint32_t stride = 0;
			for (auto& element : elements) {
				element.offset = stride;
				stride += element.size;
			}

			return stride;
		}

		std::vector<VertexBufferElement> GetLayout() const { return elements; }
		VertexBufferElement LastElement() const { return elements.back(); }
	private:
		std::vector<VertexBufferElement> elements;
	};

	class VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetVertexAttributes(VertexBufferLayout& buffer_layout) = 0;

		static std::shared_ptr<VertexArray> CreateVertexArray();
	};
}

#endif // !VERTEX_ARRAY_H
