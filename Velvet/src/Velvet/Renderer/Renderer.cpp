#include <vlpch.h>
#include "Velvet/Renderer/Renderer.h"

#include "Velvet/Renderer/Batch.h"
#include "Velvet/Renderer/Renderer2D.h"
#include "Velvet/Renderer/RendererUI.h"

#include "Velvet/Assets/Shaders/TextureShaderSource.h"

namespace Velvet {

	Scope<Renderer::SceneData> Renderer::m_SceneData = nullptr;
	Scope<ShaderLibrary> Renderer::m_ShaderLibrary = nullptr;
	Scope<Texture2DLibrary> Renderer::m_Texture2DLibrary = nullptr;

	void Renderer::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing Renderer");

		m_SceneData = CreateScope<Renderer::SceneData>();
		m_ShaderLibrary = CreateScope<ShaderLibrary>();
		m_Texture2DLibrary = CreateScope<Texture2DLibrary>();

		m_ShaderLibrary->Load("Texture", TextureShaderSource);
		m_ShaderLibrary->Load("UITexture", TextureShaderSource);
		m_Texture2DLibrary->LoadSimple("DefaultWhite", 0xffffffff);


		RenderCommand::Init();
		Batch::Init();
		Renderer2D::Init();
		RendererUI::Init();
	}

	void Renderer::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		RenderCommand::Shutdown();
		Batch::Shutdown();
		Renderer2D::Shutdown();
		RendererUI::Shutdown();
		m_ShaderLibrary->Clear();
		m_Texture2DLibrary->Clear();

		m_SceneData.reset();
		m_ShaderLibrary.reset();
		m_Texture2DLibrary.reset();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		VL_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Texture2D> texture, const Ref<VertexArray>& vertexArray, const uint32_t& indexCount, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetFloat4("u_Color", glm::vec4(1.0f));
		shader->SetMat4("u_Transform", transform);
		texture->Bind();

		RenderCommand::DrawIndexed(vertexArray, indexCount);
	}

}