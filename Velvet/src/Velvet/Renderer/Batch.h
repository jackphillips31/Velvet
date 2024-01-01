#pragma once

#include "Velvet/Renderer/Buffer.h"

#include <vector>
#include <glm/glm.hpp>

namespace Velvet {

	struct QuadVertex
	{
		QuadVertex();
		QuadVertex(glm::vec3 pos, glm::vec2 coord, glm::vec4 col);
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
	};

	struct QuadBufferElement
	{
		QuadBufferElement(glm::vec3 position, glm::mat4 transform, glm::vec4 color);
		QuadVertex x;
		QuadVertex y;
		QuadVertex z;
		QuadVertex w;
	};

	class RenderBatch
	{
	public:
		RenderBatch();
		~RenderBatch();

		void AddQuadBufferElement(QuadBufferElement& bufferElement);

		void SetLayout(BufferLayout& layout) { m_Layout = layout; }
		const BufferLayout& GetLayout() const { return m_Layout; }

		size_t GetBufferSize() const { return m_BufferSize; }
		uint32_t GetBufferCount() const { return m_BufferCount; }
		uint32_t GetQuadCount() const { return m_QuadCount; }

		Ref<VertexBuffer> CreateVertexBuffer();
		Ref<IndexBuffer> CreateIndexBuffer();
	private:
		void CalculateSizeAndCount();
		void AddBufferElement(QuadVertex& bufferElement);
	private:
		size_t m_BufferSize;
		uint32_t m_BufferCount;
		uint32_t m_QuadCount;
		std::vector<QuadVertex> m_BufferElements;
		std::vector<uint32_t> m_IndexBufferArray;
		BufferLayout m_Layout;

		Ref<VertexBuffer> m_VertexBufferObject;
		Ref<IndexBuffer> m_IndexBufferObject;
	};

}