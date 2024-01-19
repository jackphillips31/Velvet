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
		Velvet::BatchBuffer::StartAllBatches();
	}

	{
		VL_PROFILE_SCOPE("Renderer Draw");

		Velvet::Renderer2D::BeginScene(m_CameraController.GetCamera());
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
		Velvet::Renderer2D::DrawQuad(
			{ 0.0f, 0.0f, -0.1f },
			{ 10.0f, 10.0f },
			m_DefaultTexture
		);
		Velvet::Renderer2D::EndScene();
	}

	{
		VL_PROFILE_SCOPE("Renderer UI");

		Velvet::UIController::BeginScene();
		Velvet::UIController::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, Velvet::UIController::Orientation::TopLeft);
		Velvet::UIController::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, Velvet::UIController::Orientation::TopRight);
		Velvet::UIController::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, Velvet::UIController::Orientation::BottomRight);
		Velvet::UIController::AddButton({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, Velvet::UIController::Orientation::BottomLeft);
		Velvet::UIController::AddButton({ 300.0f, 300.0f }, { 1.0f, 1.0f }, m_DefaultTexture, Velvet::UIController::Orientation::TopLeft);
		Velvet::UIController::EndScene();
	}

	{
		VL_PROFILE_SCOPE("Flushing all BatchBuffers");

		Velvet::BatchBuffer::FlushAllBatches();
	}


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