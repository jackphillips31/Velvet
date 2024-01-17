#pragma once

#include "Velvet/Renderer/Batch/BufferController.h"
#include "Velvet/Renderer/Buffer.h"
#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Velvet {

	enum class BatchType
	{
		None = 0,
		Quad
	};

	class BatchBuffer
	{
	public:
		BatchBuffer(const BatchType& type, const BufferLayout& layout);
		~BatchBuffer();

		static void Init();
		static Scope<BatchBuffer> Create(const BatchType& type);
		static Scope<BatchBuffer> Create(const BatchType& type, const BufferLayout& layout);

		void AddData(const void* data, size_t size);
		// void AddQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		void StartBatch();
		void Flush();
	private:
		const BatchType m_Type;
		int m_IndicesPerElement;
		int m_VerticesPerElement;
		std::vector<uint32_t> m_IndexBufferArray;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_BatchVAO;
		Ref<Shader> m_Shader;

		Scope<BufferController> m_BufferController;
	};

}