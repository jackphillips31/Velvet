#include <vlpch.h>
#include "Velvet/Renderer/Batch.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	RenderBatch::RenderBatch(Ref<std::vector<Ref<QuadBufferElement>>> quadBuffer)
		: m_BufferSize(0), m_BufferCount(0), m_QuadCount(0), m_QuadBufferElements(quadBuffer), m_VBOExist(false), m_IBOExist(false)
	{
		VL_PROFILE_FUNCTION();
	}

	RenderBatch::~RenderBatch()
	{
		VL_PROFILE_FUNCTION();
	}

	void RenderBatch::AddBufferElement(QuadVertex& bufferElement)
	{
		VL_PROFILE_FUNCTION();

		m_VertexBufferElements.push_back(bufferElement);
	}

	void RenderBatch::AddQuadBufferElement(Ref<QuadBufferElement>& bufferElement)
	{
		VL_PROFILE_FUNCTION();

		m_QuadBufferElements->push_back(bufferElement);

		AddBufferElement(bufferElement->x);
		AddBufferElement(bufferElement->y);
		AddBufferElement(bufferElement->z);
		AddBufferElement(bufferElement->w);

		const int indicesPerQuad = 6;
		const int vertexPerQuad = 4;
		uint32_t initialIndices[indicesPerQuad] = { 0, 1, 2, 2, 3, 0 };

		for (int i = 0; i < indicesPerQuad; i++)
		{
			uint32_t offset = vertexPerQuad * m_QuadCount;
			m_IndexBufferArray.push_back(initialIndices[i] + offset);
		}

		CalculateSizeAndCount();
		m_QuadCount++;
	}

	Ref<VertexBuffer> RenderBatch::GetVertexBufferObject()
	{
		return m_VertexBufferObject;
	}

	Ref<IndexBuffer> RenderBatch::GetIndexBufferObject()
	{
		return m_IndexBufferObject;
	}

	void RenderBatch::CreateBuffers()
	{
		VL_PROFILE_FUNCTION();

		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	Ref<VertexBuffer> RenderBatch::CreateVertexBuffer()
	{
		if (!m_VBOExist)
		{
			float* result = new float[m_BufferCount];

			uint32_t index = 0;
			for (const QuadVertex& source : m_VertexBufferElements)
			{
				const size_t layoutSize = 9;
				const size_t bufferSize = layoutSize * sizeof(float);
				const size_t bufferOffset = bufferSize * index;
				float tempArray[layoutSize] = {
					source.Position.x, source.Position.y, source.Position.z,
					source.TexCoord.x, source.TexCoord.y,
					source.Color.x, source.Color.y, source.Color.z, source.Color.w
				};
				std::memcpy(reinterpret_cast<char*>(result) + bufferOffset, tempArray, bufferSize);

				index++;
			}

			m_VertexBufferObject = VertexBuffer::Create(result, (uint32_t)GetBufferSize());
			delete[] result;
			m_VBOExist = true;
		}
		return m_VertexBufferObject;
	}

	Ref<IndexBuffer> RenderBatch::CreateIndexBuffer()
	{
		if (!m_IBOExist)
		{
			const int indicesPerQuad = 6;

			m_IndexBufferObject = IndexBuffer::Create(m_IndexBufferArray.data(), GetQuadCount() * indicesPerQuad);
			m_IBOExist = true;
		}
		return m_IndexBufferObject;
	}

	void RenderBatch::CalculateSizeAndCount()
	{
		m_BufferCount = m_VertexBufferElements.size() * 9;
		m_BufferSize = m_BufferCount * 4;
	}

	QuadVertex::QuadVertex()
		: Position(0), TexCoord(0), Color(0)
	{
	}

	QuadVertex::QuadVertex(glm::vec3 pos, glm::vec2 coord, glm::vec4 col)
		: Position(pos), TexCoord(coord), Color(col)
	{
	}

	void QuadVertex::UpdatePosition(glm::vec3 pos)
	{
		Position = pos;
	}

	QuadBufferElement::QuadBufferElement(glm::vec3 position, glm::vec4 color)
	{
		VL_PROFILE_FUNCTION();

		glm::vec3& vert1 = m_DefaultVertices[0];
		glm::vec3& vert2 = m_DefaultVertices[1];
		glm::vec3& vert3 = m_DefaultVertices[2];
		glm::vec3& vert4 = m_DefaultVertices[3];

		glm::vec2 coord1(0.0f, 0.0f);
		glm::vec2 coord2(1.0f, 0.0f);
		glm::vec2 coord3(1.0f, 1.0f);
		glm::vec2 coord4(0.0f, 1.0f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

		glm::vec3 transformedPos1 = transform * glm::vec4(vert1.x, vert1.y, vert1.z, 1.0f);
		glm::vec3 transformedPos2 = transform * glm::vec4(vert2.x, vert2.y, vert2.z, 1.0f);
		glm::vec3 transformedPos3 = transform * glm::vec4(vert3.x, vert3.y, vert3.z, 1.0f);
		glm::vec3 transformedPos4 = transform * glm::vec4(vert4.x, vert4.y, vert4.z, 1.0f);

		x = QuadVertex(transformedPos1, coord1, color);
		y = QuadVertex(transformedPos2, coord2, color);
		z = QuadVertex(transformedPos3, coord3, color);
		w = QuadVertex(transformedPos4, coord4, color);
	}

	void QuadBufferElement::UpdatePosition(glm::vec3 position)
	{
		glm::vec3& vert1 = m_DefaultVertices[0];
		glm::vec3& vert2 = m_DefaultVertices[1];
		glm::vec3& vert3 = m_DefaultVertices[2];
		glm::vec3& vert4 = m_DefaultVertices[3];

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

		glm::vec3 transformedPos1 = transform * glm::vec4(vert1.x, vert1.y, vert1.z, 1.0f);
		glm::vec3 transformedPos2 = transform * glm::vec4(vert2.x, vert2.y, vert2.z, 1.0f);
		glm::vec3 transformedPos3 = transform * glm::vec4(vert3.x, vert3.y, vert3.z, 1.0f);
		glm::vec3 transformedPos4 = transform * glm::vec4(vert4.x, vert4.y, vert4.z, 1.0f);

		x.UpdatePosition(transformedPos1);
		y.UpdatePosition(transformedPos2);
		z.UpdatePosition(transformedPos3);
		w.UpdatePosition(transformedPos4);
	}

}