#include <vlpch.h>
#include "Velvet/Renderer/Batch/BufferController.h"

namespace Velvet {

	BufferController::BufferController(const size_t& elementSize, const uint32_t& maxCount, const BufferLayout& layout)
		: m_Layout(layout), m_BuffersUsed(0), m_MaxVertexCount(maxCount), m_VertexSize(elementSize)
	{
		AddNewVertexBuffer(2);

		m_BufferPtrs = m_BufferBases;
		m_BuffersUsed = 1;
	}

	BufferController::~BufferController()
	{
		DeleteAllVertexBuffers();
	}

	Scope<BufferController> BufferController::Create(const size_t& elementSize, const uint32_t& maxCount, const BufferLayout& layout)
	{
		return CreateScope<BufferController>(elementSize, maxCount, layout);
	}

	void BufferController::Reset()
	{
		m_BufferPtrs = m_BufferBases;
		m_BuffersUsed = 1;

		std::fill(m_VerticesPerBuffer.begin(), m_VerticesPerBuffer.end(), 0);
	}

	void BufferController::AddToVertexBuffer(const void* data, const size_t size)
	{
		VL_PROFILE_FUNCTION();
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

	void* BufferController::GetVertexBuffer(const int& buffer)
	{
		return m_BufferBases[buffer];
	}

	uint32_t BufferController::GetVertexBufferSize(const int& buffer)
	{
		void* currentBase = m_BufferBases[buffer];
		void* currentPtr = m_BufferPtrs[buffer];

		return (uint32_t)((uint8_t*)currentPtr - (uint8_t*)currentBase);
	}

	uint32_t BufferController::GetVertexBuffersUsed()
	{
		return m_BuffersUsed;
	}

	uint32_t BufferController::GetVertexCount(const int& buffer)
	{
		return m_VerticesPerBuffer[buffer];
	}

	Ref<VertexBuffer> BufferController::GetVBO(const int& buffer)
	{
		return m_VBOs[buffer];
	}

	void BufferController::DeleteExtraVertexBuffers()
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
	
	void BufferController::DeleteAllVertexBuffers()
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

	void BufferController::MoveVertexBufferPtr(size_t size, int count)
	{
		VL_PROFILE_FUNCTION();

		size_t bytesToMove = size * count;

		void* oldPtr = m_BufferPtrs[m_BuffersUsed - 1];
		void* nextBuffer = (void*)((char*)oldPtr + bytesToMove);

		m_BufferPtrs[m_BuffersUsed - 1] = nextBuffer;
	}

	void BufferController::AddNewVertexBuffer(int count)
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

}