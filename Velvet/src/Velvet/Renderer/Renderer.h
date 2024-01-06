#pragma once

#include "Velvet/Renderer/RenderCommand.h"

#include "Velvet/Renderer/OrthographicCamera.h"
#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/Texture.h"

#include "Velvet/Core/ResourceManager.h"

namespace Velvet {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);	// TODO
		static void EndScene();		// TODO

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f)
		);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		inline static ShaderLibrary& GetShaderLibrary() { return *m_ShaderLibrary; }
		inline static Texture2DLibrary& GetTexture2DLibrary() { return *m_Texture2DLibrary; }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> m_SceneData;
		static Scope<ShaderLibrary> m_ShaderLibrary;
		static Scope<Texture2DLibrary> m_Texture2DLibrary;
	};

}