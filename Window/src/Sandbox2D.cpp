#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Velvet::Application::Get().GetWindow()),
	m_CameraController(glm::vec2(m_Window.GetWidth(), m_Window.GetHeight())),
	m_UICameraController(glm::vec2(m_Window.GetWidth(), m_Window.GetHeight()))
{
}

void Sandbox2D::OnAttach()
{
	VL_PROFILE_FUNCTION();
}

void Sandbox2D::OnDetach()
{
	VL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Velvet::Timestep ts)
{
	VL_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);


	// Render
	{
		VL_PROFILE_SCOPE("Renderer Prep");

		Velvet::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Velvet::RenderCommand::Clear();
	}

	{
		VL_PROFILE_SCOPE("Renderer Draw");

		Velvet::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Velvet::Renderer2D::DrawQuad(
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.8f, 0.2f, 0.3f, 1.0f }
		);
		Velvet::Renderer2D::EndScene();
	}

	{
		VL_PROFILE_SCOPE("Renderer UI");

		Velvet::RendererUI::BeginScene(m_UICameraController.GetCamera());
		Velvet::RendererUI::DrawButton(
			{ 0.0f, 0.0f },
			{ 0.8f, 1.0f },
			{ 0.2f, 0.8f, 0.2f, 1.0f },
			Velvet::RendererUI::Orientation::TopLeft
		);
		Velvet::RendererUI::DrawButton(
			{ 0.0f, 0.0f },
			{ 0.8f, 1.0f },
			{ 0.2f, 0.8f, 0.2f, 1.0f },
			Velvet::RendererUI::Orientation::BottomLeft
		);
		Velvet::RendererUI::DrawButton(
			{ 0.0f, 0.0f },
			{ 0.8f, 1.0f },
			{ 0.2f, 0.2f, 0.8f, 1.0f },
			Velvet::RendererUI::Orientation::TopRight
		);
		Velvet::RendererUI::DrawButton(
			{ 0.0f, 0.0f },
			{ 0.8f, 1.0f },
			{ 0.2f, 0.2f, 0.8f, 1.0f },
			Velvet::RendererUI::Orientation::BottomRight
		);
		Velvet::RendererUI::EndScene();
	}

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	VL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Velvet::Event& e)
{
	m_CameraController.OnEvent(e);
	m_UICameraController.OnEvent(e);
}