#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Assets/Textures/DefaultTexture.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Velvet::Application::Get().GetWindow()),
	m_CameraController(glm::vec2(m_Window.GetWidth(), m_Window.GetHeight()))
{
}

void Sandbox2D::OnAttach()
{
	VL_PROFILE_FUNCTION();

	Velvet::OrthographicCameraController::CameraSettings SandboxCameraSettings;

	SandboxCameraSettings.rotation = true;
	SandboxCameraSettings.zoom = true;
	m_CameraController.SetCameraSettings(SandboxCameraSettings);

	Velvet::Ref<Velvet::RawImageData> defaultImage = Velvet::ResourceManager::LoadImage("DefaultTexture", DefaultTexturePNGData, (const int)SizeOfDefaultTexturePNGData);
	Velvet::Texture2DLibrary& textureLibrary = Velvet::Renderer::GetTexture2DLibrary();
	m_DefaultTexture = textureLibrary.Load("DefaultTexture", defaultImage);
}

void Sandbox2D::OnDetach()
{
	VL_PROFILE_FUNCTION();

	m_DefaultTexture.reset();
}

void Sandbox2D::OnUpdate(Velvet::Timestep ts)
{
	VL_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);


	// Render Prep
	{
		VL_PROFILE_SCOPE("Renderer Prep");

		Velvet::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Velvet::RenderCommand::Clear();
	}


	Velvet::Batch::StartAllBatches();

	{
		VL_PROFILE_SCOPE("Renderer2D");

		Velvet::Renderer2D::BeginScene(m_CameraController.GetCamera());
 		Velvet::Renderer2D::DrawQuad(
 			{ 0.0f, 0.0f, -0.1f },
 			{ 10.0f, 10.0f },
 			m_DefaultTexture
 		);
		Velvet::Renderer2D::DrawQuad(
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			m_SquareColor
		);
		Velvet::Renderer2D::DrawQuad(
			{ 0.5f, 0.5f },
			{ 0.2f, 0.3f },
			{ 0.4f, 0.4f, 0.2f, 1.0f }
 		);
		Velvet::Renderer2D::EndScene();
	}

	{
		VL_PROFILE_SCOPE("RendererUI");

		Velvet::RendererUI::BeginScene();
		Velvet::RendererUI::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, Velvet::RendererUI::Orientation::TopLeft);
		Velvet::RendererUI::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, Velvet::RendererUI::Orientation::TopRight);
		Velvet::RendererUI::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor, Velvet::RendererUI::Orientation::BottomRight);
		Velvet::RendererUI::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_DefaultTexture, Velvet::RendererUI::Orientation::BottomLeft);
		Velvet::RendererUI::EndScene();
	}

	Velvet::Batch::FlushAllBatches();
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
}