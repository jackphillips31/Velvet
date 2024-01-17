#include <vlpch.h>
#include "Velvet/Renderer/Batch/BatchBuffer.h"

#include "Velvet/Renderer/Texture.h"
#include "Velvet/Renderer/Renderer.h"

namespace Velvet {

	struct BatchBufferData
	{
		static const uint32_t MaxQuads = 100;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		bool isInitialized = false;

		glm::vec4 DefaultVertexPositions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 1.0f },
			{ 0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		glm::vec2 DefaultTextureCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
		int DefaultIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Ref<Shader> DefaultTextureShader;
		Ref<Texture2D> DefaultWhiteTexture;
	};

	static BatchBufferData BatchData;

	BatchBuffer::BatchBuffer(BatchType type) :
		m_Type(type),
		m_IndexBuffer(IndexBuffer::Create(BatchData.MaxIndices)),
		m_BatchVAO(VertexArray::Create()),
		m_ElementCount(0),
		m_Shader(BatchData.DefaultTextureShader)
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_ASSERT(!(type == BatchType::None), "BatchType::None is not supported!");

		switch (type)
		{
		case BatchType::None:
			m_IndicesPerElement = 0;
			m_VerticesPerElement = 0;
			break;
		case BatchType::Quad:
			m_IndicesPerElement = 6;
			m_VerticesPerElement = 4;
			VL_CORE_WARN("Creating 'Quad' BatchBuffer");
			break;
		default:
			m_IndicesPerElement = 0;
			m_VerticesPerElement = 0;
			break;
		}

		for (uint32_t i = 0; i < BatchData.MaxVertices; i++) {
			for (int i2 = 0; i2 < m_IndicesPerElement; i2++)
			{
				uint32_t offset = m_VerticesPerElement * i;
				m_IndexBufferArray.push_back(BatchData.DefaultIndices[i2] + offset);
			}
		}

		BufferLayout layout = BufferLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		});
		m_BufferController = BufferController::Create(sizeof(QuadVertexBufferElement), BatchData.MaxVertices, layout);
	}

	BatchBuffer::~BatchBuffer()
	{
		VL_PROFILE_FUNCTION();

		m_BufferController->DeleteAllVertexBuffers();
	}

	void BatchBuffer::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing BatchBuffer");

		BatchData.DefaultTextureShader = Renderer::GetShaderLibrary().Get("Texture");
		BatchData.DefaultWhiteTexture = Renderer::GetTexture2DLibrary().Get("DefaultWhite");

		BatchData.isInitialized = true;
	}

	Scope<BatchBuffer> BatchBuffer::Create(BatchType type)
	{
		VL_CORE_ASSERT(BatchData.isInitialized, "BatchData has not been initialized!");
		return CreateScope<BatchBuffer>(type);
	}

	void BatchBuffer::AddQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		VL_PROFILE_FUNCTION();

		for (size_t i = 0; i < m_VerticesPerElement; i++)
		{
			QuadVertexBufferElement bufferElement;
			bufferElement.Position = transform * BatchData.DefaultVertexPositions[i];
			bufferElement.TexCoord = BatchData.DefaultTextureCoords[i];
			bufferElement.Color = color;

			m_BufferController->AddToVertexBuffer(static_cast<void*>(&bufferElement), sizeof(QuadVertexBufferElement));
		}

		m_ElementCount++;
	}

	void BatchBuffer::StartBatch()
	{
		VL_PROFILE_FUNCTION();

		m_BufferController->Reset();
	}

	void BatchBuffer::Flush()
	{
		VL_PROFILE_FUNCTION();

		void* indexData = m_IndexBufferArray.data();

		for (uint32_t i = 0; i < m_BufferController->GetVertexBuffersUsed(); i++)
		{
			Ref<VertexBuffer> VBO = m_BufferController->GetVBO(i);

			void* vertexData = m_BufferController->GetVertexBuffer(i);
			uint32_t vertexSize = m_BufferController->GetVertexBufferSize(i);
			uint32_t indexSize = m_ElementCount * m_IndicesPerElement * sizeof(uint32_t);

			VBO->SetData(vertexData, vertexSize);
			m_IndexBuffer->SetData(indexData, indexSize);

			m_BatchVAO->AddVertexBuffer(VBO);
			m_BatchVAO->SetIndexBuffer(m_IndexBuffer);

			glm::mat4 transform = glm::mat4(1.0f);

			m_Shader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
			m_Shader->SetMat4("u_Transform", transform);
			BatchData.DefaultWhiteTexture->Bind();

			m_BatchVAO->Bind();
			RenderCommand::DrawIndexed(m_BatchVAO, (uint32_t)m_IndexBufferArray.size());
		}

		m_BufferController->DeleteExtraVertexBuffers();
		m_ElementCount = 0;
	}

}