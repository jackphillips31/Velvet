#pragma once

#include "Velvet/Renderer/Batch/BufferController.h"
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
		Ref<Texture2D> Texture;
		BufferLayout Layout;
		std::vector<int> Indices;

		BatchSettings(const BatchType& type, const Ref<Velvet::Shader>& shader, const Ref<Texture2D>& texture, const BufferLayout& layout, const std::vector<int>& indices)
			: Type(type), Shader(shader), Texture(texture), Layout(layout), Indices(indices)
		{
		}

		// Equality operator
		bool operator==(const BatchSettings& other) const
		{
			return Type == other.Type && Shader == other.Shader && Texture == other.Texture && Layout == other.Layout && Indices == other.Indices;
		}
	};

	class BatchBuffer
	{
	public:
		BatchBuffer(const BatchSettings& settings);
		~BatchBuffer();

		static void Init();
		static void Shutdown();
		static Ref<BatchBuffer> Create(const BatchSettings& settings);
		static void StartAllBatches();
		static void FlushAllBatches();

		void AddData(const void* data, size_t size);
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

		Scope<BufferController> m_BufferController;

		static bool m_IsInitialized;
		static Ref<IndexBuffer> m_IndexBuffer;
		static Ref<VertexArray> m_BatchVAO;
		static std::unordered_map<BatchSettings, Ref<BatchBuffer>> m_Instances;
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
		Velvet::hash_combine(hashValue, obj.Texture);

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