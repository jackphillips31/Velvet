#include <vlpch.h>
#include "Velvet/Renderer/Batch.h"

namespace Velvet {

	RenderBatch::RenderBatch()
		: m_BufferSize(0), m_BufferCount(0), m_QuadCount(0)
	{
	}

	RenderBatch::~RenderBatch()
	{
	}

	void RenderBatch::AddBufferElement(QuadVertex& bufferElement)
	{
		VL_PROFILE_FUNCTION();

		m_BufferElements.push_back(bufferElement);
		CalculateSizeAndCount();
	}

	void RenderBatch::AddQuadBufferElement(QuadBufferElement& bufferElement)
	{
		AddBufferElement(bufferElement.x);
		AddBufferElement(bufferElement.y);
		AddBufferElement(bufferElement.z);
		AddBufferElement(bufferElement.w);

		const int indicesPerQuad = 6;
		const int vertexPerQuad = 4;
		uint32_t initialIndices[indicesPerQuad] = { 0, 1, 2, 2, 3, 0 };

		for (int i = 0; i < indicesPerQuad; i++)
		{
			uint32_t offset = vertexPerQuad * m_QuadCount;
			m_IndexBufferArray.push_back(initialIndices[i] + offset);
		}

		m_QuadCount++;
	}

	Scope<float[]> RenderBatch::CreateVertexBuffer()
	{
		VL_PROFILE_FUNCTION();

		Scope<float[]> result = CreateScope<float[]>(m_BufferCount);

		uint32_t index = 0;
		for (const QuadVertex& source : m_BufferElements)
		{
			const size_t layoutSize = 9;
			const size_t bufferSize = layoutSize * sizeof(float);
			const size_t bufferOffset = bufferSize * index;
			float tempArray[layoutSize] = {
				source.Position.x, source.Position.y, source.Position.z,
				source.TexCoord.x, source.TexCoord.y,
				source.Color.x, source.Color.y, source.Color.z, source.Color.w
			};
			std::memcpy(reinterpret_cast<char*>(result.get()) + bufferOffset, tempArray, bufferSize);

			index++;
		}

		return result;
	}

	Scope<uint32_t[]> RenderBatch::CreateIndexBuffer()
	{
		VL_PROFILE_FUNCTION();

		const int indicesPerQuad = 6;
		const size_t indiceCount = indicesPerQuad * m_QuadCount;

		Scope<uint32_t[]> result = CreateScope<uint32_t[]>(indiceCount);

		std::memcpy(reinterpret_cast<char*>(result.get()), m_IndexBufferArray.data(), indiceCount * 4);

		return result;
	}

	void RenderBatch::CalculateSizeAndCount()
	{
		m_BufferCount = m_BufferElements.size() * 9;
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

	QuadBufferElement::QuadBufferElement(glm::vec3 position, glm::mat4 transform, glm::vec4 color)
	{
		VL_PROFILE_FUNCTION();

		float vert1[3] = { -0.5f, -0.5f, 0.0f };
		float vert2[3] = { 0.5f, -0.5f, 0.0f };
		float vert3[3] = { 0.5f, 0.5f, 0.0f };
		float vert4[3] = { -0.5f,  0.5f, 0.0f };

		glm::vec2 coord1(0.0f, 0.0f);
		glm::vec2 coord2(1.0f, 0.0f);
		glm::vec2 coord3(1.0f, 1.0f);
		glm::vec2 coord4(0.0f, 1.0f);

		glm::vec3 transformedPos1 = transform * glm::vec4(vert1[0], vert1[1], vert1[2], 1.0f);
		glm::vec3 transformedPos2 = transform * glm::vec4(vert2[0], vert2[1], vert2[2], 1.0f);
		glm::vec3 transformedPos3 = transform * glm::vec4(vert3[0], vert3[1], vert3[2], 1.0f);
		glm::vec3 transformedPos4 = transform * glm::vec4(vert4[0], vert4[1], vert4[2], 1.0f);

		x = QuadVertex(transformedPos1, coord1, color);
		y = QuadVertex(transformedPos2, coord2, color);
		z = QuadVertex(transformedPos3, coord3, color);
		w = QuadVertex(transformedPos4, coord4, color);
	}

}