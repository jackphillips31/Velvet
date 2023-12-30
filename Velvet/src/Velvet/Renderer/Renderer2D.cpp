#include <vlpch.h>
#include "Velvet/Renderer/Renderer2D.h"

#include "Velvet/Renderer/Renderer.h"
#include "Velvet/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	Scope<Renderer2D::Renderer2DStorage> Renderer2D::m_Data = nullptr;

	void Renderer2D::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing Renderer2D");

		m_Data = CreateScope<Renderer2DStorage>();
		m_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> squareVBO = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Ref<IndexBuffer> squareIBO = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		squareVBO->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});

		m_Data->QuadVertexArray->AddVertexBuffer(squareVBO);
		m_Data->QuadVertexArray->SetIndexBuffer(squareIBO);

		m_Data->WhiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		m_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_Data->TextureShader = Renderer::GetShaderLibrary().Get("Texture");
		m_Data->TextureShader->Bind();
		m_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		m_Data.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		m_Data->TextureShader->Bind();
		m_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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

		m_Data->TextureShader->SetFloat4("u_Color", color);
		m_Data->WhiteTexture->Bind();
		m_Data->TextureShader->SetMat4("u_Transform", transform);

		m_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		VL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		m_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();
		m_Data->TextureShader->SetMat4("u_Transform", transform);

		m_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data->QuadVertexArray);
	}

}