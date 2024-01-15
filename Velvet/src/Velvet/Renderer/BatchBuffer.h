#pragma once

#include "Velvet/Renderer/Buffer.h"
#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Velvet {

	struct QuadVertexBufferElement
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
	};

	enum class BatchType
	{
		None = 0,
		Quad
	};

	class BatchBuffer
	{
	public:
		BatchBuffer(BatchType type);
		~BatchBuffer();

		static void Init();
		static Scope<BatchBuffer> Create(BatchType type);

		void AddQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		void StartBatch();
		void Flush();
	private:
		QuadVertexBufferElement* GetBuffer();
		QuadVertexBufferElement* NewBatch();
		void MoveBufferPointer(QuadVertexBufferElement* initialPtr, int count = 4);
		void AddNewQuadBuffer();
	private:
		const BatchType m_Type;
		int m_IndicesPerElement;
		int m_VerticesPerElement;
		std::vector<void*> m_BufferBases;
		std::vector<void*> m_BufferPtrs;
		std::vector<Ref<VertexBuffer>> m_VBOs;
		std::vector<Ref<IndexBuffer>> m_IBOs;
		std::vector<uint32_t> m_IndexBufferArray;
		Ref<VertexArray> m_BatchVAO;
		uint32_t m_ElementCount;
		uint32_t m_BufferIndex;
		uint32_t m_BuffersUsed;
		Ref<Shader> m_Shader;
	};

}