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

		void UpdatePosition(glm::vec3 pos);
	};

	struct QuadBufferElement
	{
		QuadBufferElement(glm::vec3 position, glm::vec4 color);
		QuadVertex x;
		QuadVertex y;
		QuadVertex z;
		QuadVertex w;

		void UpdatePosition(glm::vec3 position);

		std::vector<glm::vec3> m_DefaultVertices = {
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),
			glm::vec3(-0.5f,  0.5f, 0.0f)
		};
	};

	class RenderBatch
	{
	public:
		RenderBatch(Ref<std::vector<Ref<QuadBufferElement>>> quadBuffer);
		~RenderBatch();

		void AddQuadBufferElement(Ref<QuadBufferElement>& bufferElement);

		void SetLayout(BufferLayout& layout) { m_Layout = layout; }
		const BufferLayout& GetLayout() const { return m_Layout; }

		size_t GetBufferSize() const { return m_BufferSize; }
		size_t GetBufferCount() const { return m_BufferCount; }
		uint32_t GetQuadCount() const { return m_QuadCount; }

		Ref<VertexBuffer> GetVertexBufferObject();
		Ref<IndexBuffer> GetIndexBufferObject();
		void CreateBuffers();
	private:
		Ref<VertexBuffer> CreateVertexBuffer();
		Ref<IndexBuffer> CreateIndexBuffer();
		void CalculateSizeAndCount();
		void AddBufferElement(QuadVertex& bufferElement);
	private:
		size_t m_BufferSize;
		size_t m_BufferCount;
		uint32_t m_QuadCount;
		std::vector<uint32_t> m_IndexBufferArray;
		BufferLayout m_Layout;

		Ref<VertexBuffer> m_VertexBufferObject;
		Ref<IndexBuffer> m_IndexBufferObject;

		std::vector<QuadVertex> m_VertexBufferElements;
		std::vector<QuadVertex> m_LastVertexBufferElements;

		Ref<std::vector<Ref<QuadBufferElement>>> m_QuadBufferElements;

		bool m_VBOExist;
		bool m_IBOExist;
	};

}