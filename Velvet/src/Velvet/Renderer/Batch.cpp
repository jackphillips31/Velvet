#include <vlpch.h>
#include "Velvet/Renderer/Batch.h"

#include "Velvet/Renderer/Primitives.h"
#include "Velvet/Renderer/Renderer.h"

namespace Velvet {

	struct BatchBufferData
	{
		static const uint32_t MaxQuads = 100;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;
	};

	static BatchBufferData BatchData;

	bool Batch::m_IsInitialized = false;
	Ref<IndexBuffer> Batch::m_IndexBuffer = nullptr;
	Ref<VertexArray> Batch::m_BatchVAO = nullptr;
	std::unordered_map<BatchSettings, Ref<Batch>> Batch::m_Instances;

	Batch::Batch(const BatchSettings& settings) :
		m_Settings(settings)
 	{
		VL_PROFILE_FUNCTION();

		SetDataPerElement();
		GenerateIndexBuffer();

		m_BatchBuffer = BatchBuffer::Create(m_Settings.Layout.GetStride(), BatchData.MaxVertices, m_Settings.Layout);
 	}

	Batch::~Batch()
	{
		VL_PROFILE_FUNCTION();

		m_BatchBuffer->DeleteAllVertexBuffers();
	}

	void Batch::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing Batch");

		m_IsInitialized = true;


		m_IndexBuffer = IndexBuffer::Create(BatchData.MaxIndices);
		m_BatchVAO = VertexArray::Create();
	}

	void Batch::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		m_Instances.clear();
	}

	Ref<Batch> Batch::Create(const BatchSettings& settings)
	{
		VL_CORE_ASSERT(m_IsInitialized, "BatchData has not been initialized!");

		auto it = m_Instances.find(settings);

		if (it != m_Instances.end())
		{
			return it->second;
		}
		else
		{
			Ref<Batch> result = CreateRef<Batch>(settings);
			m_Instances[settings] = result;
			return result;
		}
	}

	void Batch::StartAllBatches()
	{
		VL_PROFILE_FUNCTION();

		for (const auto& pair : m_Instances)
		{
			pair.second->StartBatch();
		}
	}

	void Batch::FlushAllBatches()
	{
		VL_PROFILE_FUNCTION();

		for (const auto& pair : m_Instances)
		{
			pair.second->Flush();
		}
	}

	void Batch::AddData(const void* data, size_t size)
	{
		VL_PROFILE_FUNCTION();

		m_BatchBuffer->AddToVertexBuffer(data, size);
	}

	void Batch::StartBatch()
	{
		m_BatchBuffer->Reset();
	}

	void Batch::Flush()
	{
		VL_PROFILE_FUNCTION();

		void* indexData = m_IndexBufferArray.data();

		for (uint32_t i = 0; i < m_BatchBuffer->GetVertexBuffersUsed(); i++)
		{
			Ref<VertexBuffer> VBO = m_BatchBuffer->GetVBO(i);

			void* vertexData = m_BatchBuffer->GetVertexBuffer(i);
			uint32_t vertexSize = m_BatchBuffer->GetVertexBufferSize(i);
			uint32_t elementCount = m_BatchBuffer->GetVertexCount(i) / m_VerticesPerElement;
			uint32_t indexCount = elementCount * m_IndicesPerElement;
			uint32_t indexSize = indexCount * sizeof(uint32_t);

			VBO->SetData(vertexData, vertexSize);
			m_IndexBuffer->SetData(indexData, indexSize);

			m_BatchVAO->AddVertexBuffer(VBO);
			m_BatchVAO->SetIndexBuffer(m_IndexBuffer);

			glm::mat4 transform = glm::mat4(1.0f);

			m_Settings.Shader->Bind();
			m_Settings.Shader->SetFloat4("u_Color", glm::vec4(1.0f));
			m_Settings.Shader->SetMat4("u_Transform", transform);
			m_Settings.Texture->Bind();

			RenderCommand::DrawIndexed(m_BatchVAO, indexCount);
		}

		m_BatchBuffer->DeleteExtraVertexBuffers();
	}

	void Batch::GenerateIndexBuffer()
	{
		VL_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < BatchData.MaxVertices; i++)
		{
			for (int i2 = 0; i2 < m_IndicesPerElement; i2++)
			{
				uint32_t offset = m_VerticesPerElement * i;
				m_IndexBufferArray.push_back(m_Settings.Indices[i2] + offset);
			}
		}
	}

	void Batch::SetDataPerElement()
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
			VL_CORE_WARN("Creating 'Quad' Batch");
			break;
		default:
			m_IndicesPerElement = 0;
			m_VerticesPerElement = 0;
			VL_CORE_ASSERT(false, "This BatchType doesn't exist!!");
			break;
		}
	}

}