#include <Velvet.h>
#include <Velvet/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Velvet::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		//--------------------------------------------------------------
		//------------------------ Draw Triangle -----------------------
		//--------------------------------------------------------------
		m_TriangleVAO = Velvet::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

		Velvet::Ref<Velvet::VertexBuffer> triangleVBO = Velvet::VertexBuffer::Create(vertices, sizeof(vertices));
		Velvet::BufferLayout layout = {
					{ Velvet::ShaderDataType::Float3, "a_Position" },
					{ Velvet::ShaderDataType::Float4, "a_Color" }
		};
		triangleVBO->SetLayout(layout);
		m_TriangleVAO->AddVertexBuffer(triangleVBO);

		uint32_t indices[3] = { 0, 1, 2 };
		Velvet::Ref<Velvet::IndexBuffer> triangleIBO = Velvet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_TriangleVAO->SetIndexBuffer(triangleIBO);

		//--------------------------------------------------------------
		//------------------------ Draw Square -------------------------
		//--------------------------------------------------------------
		m_SquareVAO = Velvet::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Velvet::Ref<Velvet::VertexBuffer> squareVBO = Velvet::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVBO->SetLayout({
			{ Velvet::ShaderDataType::Float3, "a_Position" },
			{ Velvet::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVAO->AddVertexBuffer(squareVBO);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Velvet::Ref<Velvet::IndexBuffer> squareIBO = Velvet::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVAO->SetIndexBuffer(squareIBO);

		//--------------------------------------------------------------
		//--------------------------- Shaders --------------------------
		//--------------------------------------------------------------
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader = Velvet::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		m_FlatColorShader = Velvet::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Velvet::Texture2D::Create("assets/textures/Checkerboard.png");
		m_VelvetLogoTexture = Velvet::Texture2D::Create("assets/textures/Velvet-Logo-500.png");

		std::dynamic_pointer_cast<Velvet::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Velvet::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Velvet::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Velvet::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Velvet::RenderCommand::Clear();

		Velvet::Renderer::BeginScene(m_CameraController.GetCamera());
		{
			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			std::dynamic_pointer_cast<Velvet::OpenGLShader>(m_FlatColorShader)->Bind();
			std::dynamic_pointer_cast<Velvet::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

			for (int y = 0; y < 20; y++)
			{
				for (int x = 0; x < 20; x++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Velvet::Renderer::Submit(m_FlatColorShader, m_SquareVAO, transform);
				}
			}

			auto textureShader = m_ShaderLibrary.Get("Texture");

			m_Texture->Bind();
			Velvet::Renderer::Submit(
				textureShader,
				m_SquareVAO,
				glm::scale(
					glm::mat4(1.0f),
					glm::vec3(1.5f)
				)
			);
			m_VelvetLogoTexture->Bind();
			Velvet::Renderer::Submit(
				textureShader,
				m_SquareVAO,
				glm::scale(
					glm::mat4(1.0f),
					glm::vec3(1.5f)
				)
			);

			// Triangle
			// Velvet::Renderer::Submit(m_Shader, m_TriangleVAO);
		}
		Velvet::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Velvet::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
private:
	Velvet::ShaderLibrary m_ShaderLibrary;
	Velvet::Ref<Velvet::Shader> m_Shader;
	Velvet::Ref<Velvet::VertexArray> m_TriangleVAO;

	Velvet::Ref<Velvet::Shader> m_FlatColorShader;
	Velvet::Ref<Velvet::VertexArray> m_SquareVAO;

	Velvet::Ref<Velvet::Texture2D> m_Texture, m_VelvetLogoTexture;

	Velvet::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.2f, 0.8f };
};

class WindowApp : public Velvet::Application
{
public:
	WindowApp()
	{
		PushLayer(Velvet::CreateRef<Sandbox2D>());
	}
	~WindowApp()
	{

	}
};

Velvet::Application* Velvet::CreateApplication()
{
	return new WindowApp;
}