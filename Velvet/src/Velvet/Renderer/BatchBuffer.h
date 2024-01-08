#pragma once

#include <glm/glm.hpp>

namespace Velvet {

	struct QuadVertexBufferElement
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
	};

	class BatchBuffer
	{
	public:
		static void Init();
		static void Shutdown();

		static void AddQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void StartBatch();
		static void Flush();
	private:
		static QuadVertexBufferElement* GetBuffer();
		static QuadVertexBufferElement* NewBatch();
		static void MoveBufferPointer(QuadVertexBufferElement* initialPtr, int count = 4);
		static void AddNewQuadBuffer();
	};

}