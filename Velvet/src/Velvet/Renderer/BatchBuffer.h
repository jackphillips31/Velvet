#pragma once

#include "Velvet/Renderer/Buffer.h"

namespace Velvet {

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

}