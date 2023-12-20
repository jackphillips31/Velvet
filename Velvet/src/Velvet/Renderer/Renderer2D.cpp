#include <vlpch.h>
#include "Renderer2D.h"

#include "VertexArray.h"
#include "RenderCommand.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Scope<Renderer2DStorage> s_Data;

	void Renderer2D::Init()
	{
		VL_PROFILE_FUNCTION();

		s_Data = CreateScope<Renderer2DStorage>();
		s_Data->QuadVertexArray = VertexArray::Create();

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

		s_Data->QuadVertexArray->AddVertexBuffer(squareVBO);
		s_Data->QuadVertexArray->SetIndexBuffer(squareIBO);

		s_Data->FlatColorShader = Renderer::GetShaderLibrary().Get("FlatColor");
	}

	void Renderer2D::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		s_Data.release();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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

		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}