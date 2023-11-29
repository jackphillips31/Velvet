#include <vlpch.h>
#include <Velvet.h>

#include "imgui/imgui.h"

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

		std::shared_ptr<Velvet::VertexBuffer> triangleVB;
		triangleVB.reset(Velvet::VertexBuffer::Create(vertices, sizeof(vertices)));
		Velvet::BufferLayout layout = {
					{ Velvet::ShaderDataType::Float3, "a_Position" },
					{ Velvet::ShaderDataType::Float4, "a_Color" }
		};
		triangleVB->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Velvet::IndexBuffer> triangleIB;
		triangleIB.reset(Velvet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		//--------------------------------------------------------------
		//------------------------ Draw Square -------------------------
		//--------------------------------------------------------------
		m_SquareVA.reset(Velvet::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Velvet::VertexBuffer> squareVB;
		squareVB.reset(Velvet::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Velvet::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Velvet::IndexBuffer> squareIB;
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
		m_Shader.reset(new Velvet::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
		m_BlueShader.reset(new Velvet::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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
			Velvet::Renderer::Submit(m_BlueShader, m_SquareVA);
			Velvet::Renderer::Submit(m_Shader, m_TriangleVA);
		}
		Velvet::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(Velvet::Event& event) override
	{

	}
private:
	std::shared_ptr<Velvet::Shader> m_Shader;
	std::shared_ptr<Velvet::VertexArray> m_TriangleVA;

	std::shared_ptr<Velvet::Shader> m_BlueShader;
	std::shared_ptr<Velvet::VertexArray> m_SquareVA;

	Velvet::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;
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