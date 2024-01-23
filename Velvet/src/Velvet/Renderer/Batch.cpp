#include <vlpch.h>
#include "Velvet/Renderer/Batch.h"

#include "Velvet/Renderer/Primitives.h"
#include "Velvet/Renderer/Renderer.h"

namespace Velvet {

	//--------------------------------------------------------------
	//------------------------- Batch Buffer -----------------------
	//--------------------------------------------------------------
	BatchBuffer::BatchBuffer(const size_t& elementSize, const uint32_t& maxCount, const BufferLayout& layout)
		: m_Layout(layout), m_BuffersUsed(0), m_MaxVertexCount(maxCount), m_VertexSize(elementSize)
	{
		VL_PROFILE_FUNCTION();

		AddNewVertexBuffer(2);

		m_BufferPtrs = m_BufferBases;
		m_BuffersUsed = 1;
	}

	BatchBuffer::~BatchBuffer()
	{
		VL_PROFILE_FUNCTION();

		DeleteAllVertexBuffers();
	}

	Scope<BatchBuffer> BatchBuffer::Create(const size_t& elementSize, const uint32_t& maxCount, const BufferLayout& layout)
	{
		return CreateScope<BatchBuffer>(elementSize, maxCount, layout);
	}

	void BatchBuffer::Reset()
	{
		VL_PROFILE_FUNCTION();

		m_BufferPtrs = m_BufferBases;
		m_BuffersUsed = 1;

		std::fill(m_VerticesPerBuffer.begin(), m_VerticesPerBuffer.end(), 0);
	}

	void BatchBuffer::AddToVertexBuffer(const void* data, const size_t size)
	{
		VL_CORE_ASSERT((m_VertexSize == size), "Buffer Element size does not match data size!");

		int bufferIndex = m_BuffersUsed - 1;

		void* destination = nullptr;
		if (m_VerticesPerBuffer[bufferIndex] == m_MaxVertexCount)
		{
			VL_CORE_WARN("Buffer has been filled! Adding another...");
			AddNewVertexBuffer();
			m_BuffersUsed++;
			bufferIndex = m_BuffersUsed - 1;
			m_VerticesPerBuffer[bufferIndex] = 0;
			destination = m_BufferPtrs[bufferIndex];
		}
		else
		{
			destination = m_BufferPtrs[bufferIndex];
		}

		std::memcpy(destination, data, size);
		MoveVertexBufferPtr(size);
		m_VerticesPerBuffer[bufferIndex]++;
	}

	void* BatchBuffer::GetVertexBuffer(const int& buffer)
	{
		return m_BufferBases[buffer];
	}

	uint32_t BatchBuffer::GetVertexBufferSize(const int& buffer)
	{
		void* currentBase = m_BufferBases[buffer];
		void* currentPtr = m_BufferPtrs[buffer];

		return (uint32_t)((uint8_t*)currentPtr - (uint8_t*)currentBase);
	}

	uint32_t BatchBuffer::GetVertexBuffersUsed()
	{
		return m_BuffersUsed;
	}

	uint32_t BatchBuffer::GetVertexCount(const int& buffer)
	{
		return m_VerticesPerBuffer[buffer];
	}

	Ref<VertexBuffer> BatchBuffer::GetVBO(const int& buffer)
	{
		return m_VBOs[buffer];
	}

	void BatchBuffer::DeleteExtraVertexBuffers()
	{
		VL_PROFILE_FUNCTION();

		if (m_BufferBases.size() > m_BuffersUsed + 1)
		{
			for (size_t i = m_BufferBases.size(); i > m_BuffersUsed + 1; --i)
			{
				size_t index = i - 1;
				void* itBase = m_BufferBases[index];

				free(itBase);

				m_BufferBases.erase(m_BufferBases.begin() + index);
				m_BufferPtrs.erase(m_BufferPtrs.begin() + index);
				m_VerticesPerBuffer.erase(m_VerticesPerBuffer.begin() + index);
			}
		}
	}

	void BatchBuffer::DeleteAllVertexBuffers()
	{
		VL_PROFILE_FUNCTION();

		for (size_t i = m_BufferBases.size(); i > 0; i--)
		{
			size_t index = i - 1;
			void* itBase = m_BufferBases[index];

			free(itBase);
		}
		m_BufferBases.clear();
		m_BufferPtrs.clear();
		m_VerticesPerBuffer.clear();
	}

	void BatchBuffer::MoveVertexBufferPtr(size_t size, int count)
	{
		size_t bytesToMove = size * count;

		void* oldPtr = m_BufferPtrs[m_BuffersUsed - 1];
		void* nextBuffer = (void*)((char*)oldPtr + bytesToMove);

		m_BufferPtrs[m_BuffersUsed - 1] = nextBuffer;
	}

	void BatchBuffer::AddNewVertexBuffer(int count)
	{
		VL_PROFILE_FUNCTION();

		size_t bufferSize = m_VertexSize * m_MaxVertexCount;

		for (int i = 0; i < count; i++)
		{
			void* ptr = malloc(bufferSize);
			Ref<VertexBuffer> newVBO = VertexBuffer::Create((uint32_t)bufferSize);
			newVBO->SetLayout(m_Layout);

			m_BufferBases.push_back(ptr);
			m_BufferPtrs.push_back(ptr);
			m_VerticesPerBuffer.push_back(0);
			m_VBOs.push_back(newVBO);
		}
	}

	//--------------------------------------------------------------
	//---------------------------- Batch ---------------------------
	//--------------------------------------------------------------
	struct BatchBufferData
	{
		static const uint32_t MaxQuads = 2000;
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

		int32_t samplers[BatchData.MaxTextureSlots];
		for (uint32_t i = 0; i < BatchData.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		m_Settings.Shader->Bind();
		m_Settings.Shader->SetIntArray("u_Textures", samplers, BatchData.MaxTextureSlots);

		// Set first texture slot to white texture
		m_TextureSlots[0] = Renderer::GetTexture2DLibrary().Get("DefaultWhite");
		m_TextureSlotIndex = 1;

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

	float Batch::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		VL_PROFILE_FUNCTION();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
		{
			if (*m_TextureSlots[i].get() == *texture.get())
			{
				return (float)i;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)m_TextureSlotIndex;
			m_TextureSlots[m_TextureSlotIndex] = texture;
			m_TextureSlotIndex++;
		}

		return textureIndex;

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
			if (elementCount)
			{
				uint32_t indexCount = elementCount * m_IndicesPerElement;
				uint32_t indexSize = indexCount * sizeof(uint32_t);

				VBO->SetData(vertexData, vertexSize);
				m_IndexBuffer->SetData(indexData, indexSize);

				m_BatchVAO->AddVertexBuffer(VBO);
				m_BatchVAO->SetIndexBuffer(m_IndexBuffer);

				// Bind Shader
				m_Settings.Shader->Bind();

				// Bind textures
				for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
				{
					m_TextureSlots[i]->Bind(i);
				}

				RenderCommand::DrawIndexed(m_BatchVAO, indexCount);
			}
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