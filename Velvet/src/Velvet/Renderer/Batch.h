#pragma once

#include "Velvet/Renderer/Buffer.h"
#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/Texture.h"
#include "Velvet/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Velvet {

	enum class BatchType
	{
		None = 0,
		Quad
	};

	struct BatchSettings
	{
		BatchType Type;
		Ref<Shader> Shader;
		BufferLayout Layout;
		std::vector<int> Indices;

		BatchSettings(const BatchType& type, const Ref<Velvet::Shader>& shader, const BufferLayout& layout, const std::vector<int>& indices)
			: Type(type), Shader(shader), Layout(layout), Indices(indices)
		{
		}

		// Equality operator
		bool operator==(const BatchSettings& other) const
		{
			return Type == other.Type && Shader == other.Shader && Layout == other.Layout && Indices == other.Indices;
		}
	};

	class BatchBuffer
	{
	public:
		BatchBuffer(const size_t& elementSize, const uint32_t& maxCount, const BufferLayout& layout);
		~BatchBuffer();

		static Scope<BatchBuffer> Create(const size_t& elementSize, const uint32_t& maxCount, const BufferLayout& layout);

		void Reset();
		void AddToVertexBuffer(const void* data, const size_t size);
		void* GetVertexBuffer(const int& buffer);
		uint32_t GetVertexBufferSize(const int& buffer);
		uint32_t GetVertexBuffersUsed();
		uint32_t GetVertexCount(const int& buffer);
		Ref<VertexBuffer> GetVBO(const int& buffer);
		void DeleteExtraVertexBuffers();
		void DeleteAllVertexBuffers();
	private:
		void MoveVertexBufferPtr(size_t size, int count = 1);
		void AddNewVertexBuffer(int count = 1);
		void GenerateIndexBuffer();
	private:
		BufferLayout m_Layout;
		std::vector<void*> m_BufferBases;
		std::vector<void*> m_BufferPtrs;
		std::vector<uint32_t> m_VerticesPerBuffer;
		std::vector<Ref<VertexBuffer>> m_VBOs;
		uint32_t m_BuffersUsed;
		uint32_t m_MaxVertexCount;
		size_t m_VertexSize;
	};

	class Batch
	{
	public:
		Batch(const BatchSettings& settings);
		~Batch();

		static void Init();
		static void Shutdown();
		static Ref<Batch> Create(const BatchSettings& settings);
		static void StartAllBatches();
		static void FlushAllBatches();

		void AddData(const void* data, size_t size);
		float GetTextureIndex(const Ref<Texture2D>& texture);
	private:
		void StartBatch();
		void Flush();
		void GenerateIndexBuffer();
		void SetDataPerElement();
	private:
		const BatchSettings m_Settings;
		int m_IndicesPerElement = 0;
		int m_VerticesPerElement = 0;
		std::vector<uint32_t> m_IndexBufferArray;
		std::array<Ref<Texture2D>, 32> m_TextureSlots;
		uint32_t m_TextureSlotIndex;

		Scope<BatchBuffer> m_BatchBuffer;

		static bool m_IsInitialized;
		static Ref<IndexBuffer> m_IndexBuffer;
		static Ref<VertexArray> m_BatchVAO;
		static std::unordered_map<BatchSettings, Ref<Batch>> m_Instances;
	};

}

// Hash operator so that BatchSettings can be used 
// with an std::unordered_map!
template<>
struct std::hash<Velvet::BatchSettings>
{
	size_t operator()(const Velvet::BatchSettings& obj) const
	{
		size_t hashValue = 0;

		// Hash BatchType
		Velvet::hash_combine(hashValue, std::hash<int>{}(static_cast<int>(obj.Type)));

		// Hash shared pointers
		Velvet::hash_combine(hashValue, obj.Shader);

		// Hash BufferLayout
		Velvet::hash_combine(hashValue, std::hash<Velvet::BufferLayout>{}(obj.Layout));

		// Hash Vector
		for (const int& elem : obj.Indices)
		{
			Velvet::hash_combine(hashValue, elem);
		}

		return hashValue;
	}
};