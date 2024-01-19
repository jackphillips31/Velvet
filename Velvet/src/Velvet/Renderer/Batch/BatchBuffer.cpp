#include <vlpch.h>
#include "Velvet/Renderer/Batch/BatchBuffer.h"

#include "Velvet/Renderer/Primitives.h"
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

		Ref<Shader> DefaultTextureShader;
		Ref<Texture2D> DefaultWhiteTexture;
	};

	static BatchBufferData BatchData;

	Ref<IndexBuffer> BatchBuffer::m_IndexBuffer = nullptr;
	Ref<VertexArray> BatchBuffer::m_BatchVAO = nullptr;
	std::unordered_map<BatchSettings, Ref<BatchBuffer>> BatchBuffer::m_Instances;

	BatchBuffer::BatchBuffer(const BatchSettings& settings) :
		m_Settings(settings)
 	{
		SetDataPerElement();
		GenerateIndexBuffer();

		m_BufferController = BufferController::Create(m_Settings.Layout.GetStride(), BatchData.MaxVertices, m_Settings.Layout);
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


		m_IndexBuffer = IndexBuffer::Create(BatchData.MaxIndices);
		m_BatchVAO = VertexArray::Create();
	}

	void BatchBuffer::Shutdown()
	{
		m_Instances.clear();
	}

	Ref<BatchBuffer> BatchBuffer::Create(const BatchSettings& settings)
	{
		VL_CORE_ASSERT(BatchData.isInitialized, "BatchData has not been initialized!");

		auto it = m_Instances.find(settings);

		if (it != m_Instances.end())
		{
			return it->second;
		}
		else
		{
			Ref<BatchBuffer> result = CreateRef<BatchBuffer>(settings);
			m_Instances[settings] = result;
			return result;
		}
	}

	void BatchBuffer::StartAllBatches()
	{
		for (const auto& pair : m_Instances)
		{
			pair.second->StartBatch();
		}
	}

	void BatchBuffer::FlushAllBatches()
	{
		for (const auto& pair : m_Instances)
		{
			pair.second->Flush();
		}
	}

	void BatchBuffer::StartBatch()
	{
		VL_PROFILE_FUNCTION();

		m_BufferController->Reset();
	}

	void BatchBuffer::AddData(const void* data, size_t size)
	{
		m_BufferController->AddToVertexBuffer(data, size);
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
			uint32_t elementCount = m_BufferController->GetVertexCount(i) / m_VerticesPerElement;
			uint32_t indexSize = elementCount * m_IndicesPerElement * sizeof(uint32_t);

			VBO->SetData(vertexData, vertexSize);
			m_IndexBuffer->SetData(indexData, indexSize);

			m_BatchVAO->AddVertexBuffer(VBO);
			m_BatchVAO->SetIndexBuffer(m_IndexBuffer);

			glm::mat4 transform = glm::mat4(1.0f);

			m_Settings.Shader->SetFloat4("u_Color", glm::vec4(1.0f));
			m_Settings.Shader->SetMat4("u_Transform", transform);
			m_Settings.Texture->Bind();

			m_BatchVAO->Bind();
			RenderCommand::DrawIndexed(m_BatchVAO, (uint32_t)m_IndexBufferArray.size());
		}

		m_BufferController->DeleteExtraVertexBuffers();
	}

	void BatchBuffer::GenerateIndexBuffer()
	{
		for (uint32_t i = 0; i < BatchData.MaxVertices; i++)
		{
			for (int i2 = 0; i2 < m_IndicesPerElement; i2++)
			{
				uint32_t offset = m_VerticesPerElement * i;
				m_IndexBufferArray.push_back(m_Settings.Indices[i2] + offset);
			}
		}
	}

	void BatchBuffer::SetDataPerElement()
	{
		switch (m_Settings.Type)
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
			VL_CORE_ASSERT(false, "This BatchType doesn't exist!!");
			break;
		}
	}

}