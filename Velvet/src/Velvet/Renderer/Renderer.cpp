#include <vlpch.h>
#include "Renderer.h"

#include "Renderer2D.h"
#include "UIController.h"

#include "Velvet/Assets/Shaders/FlatColorShaderSource.h"
#include "Velvet/Assets/Shaders/TextureShaderSource.h"

namespace Velvet {

	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();
	Scope<ShaderLibrary> Renderer::m_ShaderLibrary = CreateScope<ShaderLibrary>();

	void Renderer::Init()
	{
		VL_PROFILE_FUNCTION();

		m_ShaderLibrary->Load("FlatColor", FlatColorShaderSource);
		m_ShaderLibrary->Load("Texture", TextureShaderSource);

		RenderCommand::Init();
		Renderer2D::Init();
		UIController::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		UIController::Shutdown();
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