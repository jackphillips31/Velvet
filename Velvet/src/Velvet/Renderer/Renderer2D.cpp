#include <vlpch.h>
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	Scope<Renderer2D::Renderer2DStorage> Renderer2D::m_Data = nullptr;

	void Renderer2D::Init()
	{
		VL_PROFILE_FUNCTION();

		m_Data = CreateScope<Renderer2DStorage>();
		m_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> squareVBO = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Ref<IndexBuffer> squareIBO = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		squareVBO->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});

		m_Data->QuadVertexArray->AddVertexBuffer(squareVBO);
		m_Data->QuadVertexArray->SetIndexBuffer(squareIBO);

		m_Data->FlatColorShader = Renderer::GetShaderLibrary().Get("FlatColor");
	}

	void Renderer2D::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		m_Data.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		m_Data->FlatColorShader->Bind();
		m_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		VL_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		VL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		m_Data->FlatColorShader->Bind();
		m_Data->FlatColorShader->SetFloat4("u_Color", color);
		m_Data->FlatColorShader->SetMat4("u_Transform", transform);

		m_Data->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(m_Data->QuadVertexArray);
	}

}