#include <vlpch.h>
#include "Velvet/Renderer/Renderer2D.h"

#include "Velvet/Renderer/Batch.h"
#include "Velvet/Renderer/Primitives.h"
#include "Velvet/Renderer/Renderer.h"
#include "Velvet/Renderer/Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	struct Render2DData
	{
		Ref<Batch> Batch;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		BufferLayout BatchLayout = BufferLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		});
	};

	static Render2DData s_Data;

	void Renderer2D::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing Renderer2D");

		s_Data.TextureShader = Renderer::GetShaderLibrary().Get("Texture");
		s_Data.WhiteTexture = Renderer::GetTexture2DLibrary().Get("DefaultWhite");

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);

		BatchSettings Renderer2DBatchSettings(
			BatchType::Quad,
			s_Data.TextureShader,
			s_Data.WhiteTexture,
			s_Data.BatchLayout,
			Primitives::Quad::Indices
		);

		s_Data.Batch = Batch::Create(Renderer2DBatchSettings);
	}

	void Renderer2D::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		s_Data.Batch.reset();
		s_Data.TextureShader.reset();
		s_Data.WhiteTexture.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Primitives::QuadVertex bufferElement;
			bufferElement.Position = transform * Primitives::Quad::Vertices[i];
			bufferElement.TexCoord = Primitives::Quad::TextureCoords[i];
			bufferElement.Color = color;

			s_Data.Batch->AddData(static_cast<void*>(&bufferElement), sizeof(Primitives::QuadVertex));
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		VL_PROFILE_FUNCTION();

		BatchSettings textureBatchSettings(
			BatchType::Quad,
			s_Data.TextureShader,
			texture,
			s_Data.BatchLayout,
			Primitives::Quad::Indices
		);
		Ref<Batch> textureBatch = Batch::Create(textureBatchSettings);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Primitives::QuadVertex bufferElement;
			bufferElement.Position = transform * Primitives::Quad::Vertices[i];
			bufferElement.TexCoord = Primitives::Quad::TextureCoords[i];
			bufferElement.Color = glm::vec4(1.0f);

			textureBatch->AddData(static_cast<void*>(&bufferElement), sizeof(Primitives::QuadVertex));
		}
	}

}