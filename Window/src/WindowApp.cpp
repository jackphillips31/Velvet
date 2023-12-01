#include <vlpch.h>
#include <Velvet.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Velvet::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		//--------------------------------------------------------------
		//------------------------ Draw Triangle -----------------------
		//--------------------------------------------------------------
		m_TriangleVA.reset(Velvet::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

		Velvet::Ref<Velvet::VertexBuffer> triangleVB;
		triangleVB.reset(Velvet::VertexBuffer::Create(vertices, sizeof(vertices)));
		Velvet::BufferLayout layout = {
					{ Velvet::ShaderDataType::Float3, "a_Position" },
					{ Velvet::ShaderDataType::Float4, "a_Color" }
		};
		triangleVB->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		Velvet::Ref<Velvet::IndexBuffer> triangleIB;
		triangleIB.reset(Velvet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		//--------------------------------------------------------------
		//------------------------ Draw Square -------------------------
		//--------------------------------------------------------------
		m_SquareVA.reset(Velvet::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Velvet::Ref<Velvet::VertexBuffer> squareVB;
		squareVB.reset(Velvet::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Velvet::ShaderDataType::Float3, "a_Position" },
			{ Velvet::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Velvet::Ref<Velvet::IndexBuffer> squareIB;
		squareIB.reset(Velvet::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

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
		m_Shader.reset(Velvet::Shader::Create(vertexSrc, fragmentSrc));

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
		m_FlatColorShader.reset(Velvet::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string textureShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";
		m_TextureShader.reset(Velvet::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Velvet::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Velvet::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Velvet::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Velvet::Timestep ts) override
	{
		if (Velvet::Input::IsKeyPressed(VL_KEY_LEFT) || Velvet::Input::IsKeyPressed(VL_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Velvet::Input::IsKeyPressed(VL_KEY_RIGHT) || Velvet::Input::IsKeyPressed(VL_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Velvet::Input::IsKeyPressed(VL_KEY_UP) || Velvet::Input::IsKeyPressed(VL_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Velvet::Input::IsKeyPressed(VL_KEY_DOWN) || Velvet::Input::IsKeyPressed(VL_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Velvet::Input::IsKeyPressed(VL_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Velvet::Input::IsKeyPressed(VL_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Velvet::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Velvet::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Velvet::Renderer::BeginScene(m_Camera);
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
					Velvet::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
				}
			}

			m_Texture->Bind();
			Velvet::Renderer::Submit(
				m_TextureShader,
				m_SquareVA,
				glm::scale(
					glm::mat4(1.0f),
					glm::vec3(1.5f)
				)
			);

			// Triangle
			// Velvet::Renderer::Submit(m_Shader, m_TriangleVA);
		}
		Velvet::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Velvet::Event& event) override
	{

	}
private:
	Velvet::Ref<Velvet::Shader> m_Shader;
	Velvet::Ref<Velvet::VertexArray> m_TriangleVA;

	Velvet::Ref<Velvet::Shader> m_FlatColorShader, m_TextureShader;
	Velvet::Ref<Velvet::VertexArray> m_SquareVA;

	Velvet::Ref<Velvet::Texture2D> m_Texture;

	Velvet::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.2f, 0.8f };
};

class WindowApp : public Velvet::Application
{
public:
	WindowApp()
	{
		PushLayer(new ExampleLayer());
	}
	~WindowApp()
	{

	}
};

Velvet::Application* Velvet::CreateApplication()
{
	return new WindowApp;
}