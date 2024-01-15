#include <vlpch.h>
#include "Velvet/Renderer/BatchBuffer.h"

#include "Velvet/Renderer/Texture.h"
#include "Velvet/Renderer/Renderer.h"

namespace Velvet {

	struct BatchBufferData
	{
		static const uint32_t MaxQuads = 20000;
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

	BatchBuffer::BatchBuffer(BatchType type)
		: m_Type(type), m_BatchVAO(VertexArray::Create())
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

		m_Shader = BatchData.DefaultTextureShader;
	}

	BatchBuffer::~BatchBuffer()
	{
		VL_PROFILE_FUNCTION();

		// We can use this to delete the buffers
		for (size_t i = m_BufferBases.size(); i > 0; i--)
		{
			auto itBase = m_BufferBases.begin() + i - 1;
			auto itPtr = m_BufferPtrs.begin() + i - 1;

			delete[] * itBase;
			m_BufferBases.erase(itBase);
			m_BufferPtrs.erase(itPtr);
		}

		BatchData.DefaultTextureShader.reset();
		BatchData.DefaultWhiteTexture.reset();
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

		QuadVertexBufferElement* currentBuffer = GetBuffer();

		for (size_t i = 0; i < m_VerticesPerElement; i++)
		{
			currentBuffer->Position = transform * BatchData.DefaultVertexPositions[i];
			currentBuffer->Color = color;
			currentBuffer->TexCoord = BatchData.DefaultTextureCoords[i];

			currentBuffer++;
		}

		for (int i = 0; i < m_IndicesPerElement; i++)
		{
			uint32_t offset = m_VerticesPerElement * m_ElementCount;
			m_IndexBufferArray.push_back(BatchData.DefaultIndices[i] + offset);
		}

		m_ElementCount++;
		m_BufferIndex += m_VerticesPerElement;
	}

	void BatchBuffer::StartBatch()
	{
		VL_PROFILE_FUNCTION();

		if (m_BufferBases.size() == 0 && m_BufferPtrs.size() == 0)
		{
			AddNewQuadBuffer();
			AddNewQuadBuffer();
		}

		m_BufferPtrs = m_BufferBases;

		m_ElementCount = 0;
		m_BufferIndex = 0;
		m_BuffersUsed = 1;
	}

	void BatchBuffer::Flush()
	{
		VL_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < m_BuffersUsed; i++)
		{
			QuadVertexBufferElement* currentBase = (QuadVertexBufferElement*)m_BufferBases[i];
			QuadVertexBufferElement* currentPtr = (QuadVertexBufferElement*)m_BufferPtrs[i];

			uint32_t bufferSize = (uint32_t)((uint8_t*)currentPtr - (uint8_t*)currentBase);
			m_VBOs[i]->SetData(m_BufferBases[i], bufferSize);

			Ref<IndexBuffer> newIBO = IndexBuffer::Create(m_IndexBufferArray.data(), m_ElementCount * m_IndicesPerElement);
			
			m_BatchVAO->AddVertexBuffer(m_VBOs[i]);
			m_BatchVAO->SetIndexBuffer(newIBO);

			glm::mat4 transform = glm::mat4(1.0f);

			m_Shader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
			m_Shader->SetMat4("u_Transform", transform);
			BatchData.DefaultWhiteTexture->Bind();

			m_BatchVAO->Bind();
			RenderCommand::DrawIndexed(m_BatchVAO, (uint32_t)m_IndexBufferArray.size());
		}

		// Delete any extra buffers if they exist
		if (m_BufferBases.size() > m_BuffersUsed + 1)
		{
			for (size_t i = m_BufferBases.size(); i > m_BuffersUsed + 1; --i)
			{
				auto itBase = m_BufferBases.begin() + i;
				auto itPtr = m_BufferPtrs.begin() + i;

				delete[] *itBase;
				delete[] *itPtr;
				m_BufferBases.erase(itBase);
				m_BufferPtrs.erase(itPtr);
			}
		}
	}

	QuadVertexBufferElement* BatchBuffer::GetBuffer()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_ASSERT(m_BuffersUsed, "StartBatch has not been run!");

		if (m_BufferIndex == BatchData.MaxVertices)
		{
			return NewBatch();
		}
		else
		{
			QuadVertexBufferElement* currentBuffer = (QuadVertexBufferElement*)m_BufferPtrs[m_BuffersUsed - 1];
			MoveBufferPointer(currentBuffer);

			return currentBuffer;
		}
	}

	QuadVertexBufferElement* BatchBuffer::NewBatch()
	{
		VL_PROFILE_FUNCTION();

		m_BufferIndex = 0;
		m_BuffersUsed++;
		m_IndexBufferArray.clear();

		AddNewQuadBuffer();

		QuadVertexBufferElement* currentBuffer = (QuadVertexBufferElement*)m_BufferPtrs[m_BuffersUsed - 1];
		MoveBufferPointer(currentBuffer);

		return currentBuffer;
	}

	void BatchBuffer::MoveBufferPointer(QuadVertexBufferElement* initialPtr, int count)
	{
		VL_PROFILE_FUNCTION();

		QuadVertexBufferElement* nextBuffer = initialPtr += 4;
		m_BufferPtrs[m_BuffersUsed - 1] = nextBuffer;
	}

	void BatchBuffer::AddNewQuadBuffer()
	{
		VL_PROFILE_FUNCTION();

		QuadVertexBufferElement* newBuffer = new QuadVertexBufferElement[BatchData.MaxVertices];
		Ref<VertexBuffer> newVBO = VertexBuffer::Create(BatchData.MaxVertices * sizeof(QuadVertexBufferElement));
		newVBO->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		});

		m_BufferBases.push_back(newBuffer);
		m_BufferPtrs.push_back(newBuffer);
		m_VBOs.push_back(newVBO);
	}

}