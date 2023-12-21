#pragma once

#include "OrthographicCamera.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Velvet {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	private:
		struct Renderer2DStorage
		{
			Ref<VertexArray> QuadVertexArray;
			Ref<Shader> FlatColorShader;
		};

		static Scope<Renderer2DStorage> m_Data;
	};

}