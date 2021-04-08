#ifndef BUFFER_H
#define BUFFER_H

#include <memory>

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

	static uint32_t GetSizeInBytes(VertexShaderType type) {
		switch (type)
		{
		case Pixel::VertexShaderType::None: return 0;
			break;
		case Pixel::VertexShaderType::Float: return 4;
			break;
		case Pixel::VertexShaderType::Int: return 4;
			break;
		default:
			break;
		}

		return 0;
	}

	class VertexBufferLayout {
	public:
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
			: elements(elements) {
			Calculate();
		}

		VertexBufferLayout() { }

		void AddToBuffer(const VertexBufferElement& element) {
			elements.push_back(element);
			elements.back().index = (uint32_t)(elements.size() - 1);
		}

		uint32_t Calculate() {
			stride = 0;
			for (auto& element : elements) {
				element.offset = stride;
				stride += element.size;
			}

			return stride;
		}

		std::vector<VertexBufferElement> GetLayout() const { return elements; }
		VertexBufferElement LastElement() const { return elements.back(); }

		uint32_t GetStride() const { return stride; }
	private:
		std::vector<VertexBufferElement> elements;
		uint32_t stride = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual uint32_t GetId() const = 0;

		virtual void SetLayout(const VertexBufferLayout& lay) = 0;
		virtual std::shared_ptr<VertexBufferLayout> GetLayout() = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		static std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t size);
		static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size);
	};

	class IndexBuffer {  
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual uint32_t GetId() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual void SetData(uint32_t* data, uint32_t size) = 0;

		static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t size);
		static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t size);
	};

	class UniformBuffer {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual uint32_t GetId() const = 0;
		virtual void SetData(void* data, uint32_t size, uint32_t offset) = 0;
		virtual void AllocateData(uint32_t size) = 0;

		virtual uint32_t GetUniformBlockId(uint32_t shader_id, const std::string& block_name) = 0;
		virtual void BindToBindPoint() = 0;
		virtual void BindToShader(uint32_t shader_id, const std::string& block_name) = 0;

		static std::shared_ptr<UniformBuffer> CreateUnifromBuffer(uint32_t size);
	};

	class IndirectDrawBuffer {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual uint32_t GetId() const = 0;
		virtual void SetData(void* data, uint32_t size, uint32_t offset) = 0;
		virtual void AllocateData(uint32_t size, void* data) = 0;

		static std::shared_ptr<IndirectDrawBuffer> CreateIndirectDrawBuffer(uint32_t size);
	};

	class ShaderStorageBuffer {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual uint32_t GetId() const = 0;
		virtual void SetData(void* data, uint32_t size, uint32_t offset) = 0;
		virtual void AllocateData(uint32_t size, void* data) = 0;

		virtual uint32_t GetUniformBlockId(uint32_t shader_id, const std::string& block_name) = 0;
		virtual void BindToBindPoint() = 0;
		virtual void BindToShader(uint32_t shader_id, const std::string& block_name) = 0;

		static std::shared_ptr<ShaderStorageBuffer> CreateShaderStorageBuffer(uint32_t size);
	};
}

#endif // !BUFFER_H
