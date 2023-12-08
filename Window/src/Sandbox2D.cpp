#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVAO = Velvet::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Velvet::Ref<Velvet::VertexBuffer> squareVBO;

	squareVBO.reset(Velvet::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVBO->SetLayout({
		{ Velvet::ShaderDataType::Float3, "a_Position" }
	});
	m_SquareVAO->AddVertexBuffer(squareVBO);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Velvet::Ref<Velvet::IndexBuffer> squareIBO;

	squareIBO.reset(Velvet::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVAO->SetIndexBuffer(squareIBO);

	m_FlatColorShader = Velvet::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Velvet::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Velvet::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Velvet::RenderCommand::Clear();

	Velvet::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Velvet::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Velvet::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Velvet::Renderer::Submit(m_FlatColorShader, m_SquareVAO, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Velvet::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Velvet::Event& e)
{
	m_CameraController.OnEvent(e);
}