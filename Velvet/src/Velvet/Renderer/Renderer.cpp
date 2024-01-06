#include <vlpch.h>
#include "Velvet/Renderer/Renderer.h"

#include "Velvet/Renderer/Renderer2D.h"
#include "Velvet/Renderer/UIController.h"

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
		m_Texture2DLibrary->LoadSimple("DefaultWhite", 0xffffffff);


		RenderCommand::Init();
		Renderer2D::Init();
		UIController::Init();
	}

	void Renderer::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
		UIController::Shutdown();
		m_SceneData.reset();
		m_ShaderLibrary.reset();
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

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}