#include <vlpch.h>
#include "Velvet/Renderer/BatchBuffer.h"

#include "Velvet/Renderer/Buffer.h"
#include "Velvet/Renderer/VertexArray.h"

#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/Texture.h"
#include "Velvet/Renderer/Renderer.h"

namespace Velvet {

	struct BatchBufferData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;
		static const int IndicesPerQuad = 6;
		static const int VertexPerQuad = 4;

		glm::vec4 QuadVertexPositions[4];
		glm::vec2 TextureCoords[4];
		int InitialIndices[IndicesPerQuad];
		std::vector<QuadVertexBufferElement*> QuadBufferBases;
		std::vector<QuadVertexBufferElement*> QuadBufferPtrs;
		std::vector<Ref<VertexBuffer>> QuadVBOs;
		std::vector<Ref<IndexBuffer>> QuadIBOs;
		std::vector<uint32_t> IndexBufferArray;
		Ref<VertexArray> BatchVAO;
		uint32_t QuadCount = 0;
		uint32_t BufferIndex = 0;
		uint32_t BuffersUsed = 0;

		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static BatchBufferData BatchData;

	void BatchBuffer::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing BatchBuffer");

		BatchData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		BatchData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		BatchData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		BatchData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		BatchData.TextureCoords[0] = { 0.0f, 0.0f };
		BatchData.TextureCoords[1] = { 1.0f, 0.0f };
		BatchData.TextureCoords[2] = { 1.0f, 1.0f };
		BatchData.TextureCoords[3] = { 0.0f, 1.0f };

		BatchData.InitialIndices[0] = 0;
		BatchData.InitialIndices[1] = 1;
		BatchData.InitialIndices[2] = 2;
		BatchData.InitialIndices[3] = 2;
		BatchData.InitialIndices[4] = 3;
		BatchData.InitialIndices[5] = 0;

		BatchData.BatchVAO = VertexArray::Create();

		BatchData.TextureShader = Renderer::GetShaderLibrary().Get("Texture");
		BatchData.WhiteTexture = Renderer::GetTexture2DLibrary().Get("DefaultWhite");
	}

	void BatchBuffer::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		// We can use this to delete the buffers
		for (size_t i = BatchData.QuadBufferBases.size(); i > 0; i--)
		{
			auto itBase = BatchData.QuadBufferBases.begin() + i - 1;
			auto itPtr = BatchData.QuadBufferPtrs.begin() + i - 1;

			delete[] *itBase;
			BatchData.QuadBufferBases.erase(itBase);
			BatchData.QuadBufferPtrs.erase(itPtr);
		}

		// Or this one
		/*
		while (!BatchData.QuadBufferBases.empty())
		{
			QuadVertexBufferElement* ptr = BatchData.QuadBufferBases.back();
			delete[] ptr;
			BatchData.QuadBufferBases.pop_back();
		}
		while (!BatchData.QuadBufferPtrs.empty())
		{
			BatchData.QuadBufferPtrs.pop_back();
		}
		*/
	}

	void BatchBuffer::AddQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		VL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		QuadVertexBufferElement* currentBuffer = GetBuffer();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			currentBuffer->Position = transform * BatchData.QuadVertexPositions[i];
			currentBuffer->Color = color;
			currentBuffer->TexCoord = BatchData.TextureCoords[i];

			currentBuffer++;
		}

		for (int i = 0; i < BatchData.IndicesPerQuad; i++)
		{
			uint32_t offset = BatchData.VertexPerQuad * BatchData.QuadCount;
			BatchData.IndexBufferArray.push_back(BatchData.InitialIndices[i] + offset);
		}

		BatchData.QuadCount++;
		BatchData.BufferIndex += quadVertexCount;
	}

	void BatchBuffer::StartBatch()
	{
		VL_PROFILE_FUNCTION();

		if (BatchData.QuadBufferBases.size() == 0 && BatchData.QuadBufferPtrs.size() == 0)
		{
			AddNewQuadBuffer();
			AddNewQuadBuffer();
		}

		BatchData.QuadBufferPtrs = BatchData.QuadBufferBases;

		BatchData.QuadCount = 0;
		BatchData.BufferIndex = 0;
		BatchData.BuffersUsed = 1;
	}

	void BatchBuffer::Flush()
	{
		VL_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < BatchData.BuffersUsed; i++)
		{
			QuadVertexBufferElement* currentBase = BatchData.QuadBufferBases[i];
			QuadVertexBufferElement* currentPtr = BatchData.QuadBufferPtrs[i];

			uint32_t bufferSize = (uint32_t)((uint8_t*)currentPtr - (uint8_t*)currentBase);
			BatchData.QuadVBOs[i]->SetData(BatchData.QuadBufferBases[i], bufferSize);

			Ref<IndexBuffer> newIBO = IndexBuffer::Create(BatchData.IndexBufferArray.data(), BatchData.QuadCount * BatchData.IndicesPerQuad);
			
			BatchData.BatchVAO->AddVertexBuffer(BatchData.QuadVBOs[i]);
			BatchData.BatchVAO->SetIndexBuffer(newIBO);

			glm::mat4 transform = glm::mat4(1.0f);

			BatchData.TextureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
			BatchData.TextureShader->SetMat4("u_Transform", transform);
			BatchData.WhiteTexture->Bind();

			BatchData.BatchVAO->Bind();
			RenderCommand::DrawIndexed(BatchData.BatchVAO, (uint32_t)BatchData.IndexBufferArray.size());
		}

		// Delete any extra buffers if they exist
		if (BatchData.QuadBufferBases.size() > BatchData.BuffersUsed + 1)
		{
			for (size_t i = BatchData.QuadBufferBases.size(); i > BatchData.BuffersUsed + 1; --i)
			{
				auto itBase = BatchData.QuadBufferBases.begin() + i;
				auto itPtr = BatchData.QuadBufferPtrs.begin() + i;

				delete[] *itBase;
				delete[] *itPtr;
				BatchData.QuadBufferBases.erase(itBase);
				BatchData.QuadBufferPtrs.erase(itPtr);
			}
		}
	}

	QuadVertexBufferElement* BatchBuffer::GetBuffer()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_ASSERT(BatchData.BuffersUsed, "StartBatch has not been run!");

		if (BatchData.BufferIndex == BatchData.MaxVertices)
		{
			return NewBatch();
		}
		else
		{
			QuadVertexBufferElement* currentBuffer = BatchData.QuadBufferPtrs[BatchData.BuffersUsed - 1];
			MoveBufferPointer(currentBuffer);

			return currentBuffer;
		}
	}

	QuadVertexBufferElement* BatchBuffer::NewBatch()
	{
		VL_PROFILE_FUNCTION();

		BatchData.BufferIndex = 0;
		BatchData.BuffersUsed++;
		BatchData.IndexBufferArray.clear();

		AddNewQuadBuffer();

		QuadVertexBufferElement* currentBuffer = BatchData.QuadBufferPtrs[BatchData.BuffersUsed - 1];
		MoveBufferPointer(currentBuffer);

		return currentBuffer;
	}

	void BatchBuffer::MoveBufferPointer(QuadVertexBufferElement* initialPtr, int count)
	{
		VL_PROFILE_FUNCTION();

		QuadVertexBufferElement* nextBuffer = initialPtr += 4;
		BatchData.QuadBufferPtrs[BatchData.BuffersUsed - 1] = nextBuffer;
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

		BatchData.QuadBufferBases.push_back(newBuffer);
		BatchData.QuadBufferPtrs.push_back(newBuffer);
		BatchData.QuadVBOs.push_back(newVBO);
	}

}