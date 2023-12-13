#include <vlpch.h>
#include "RendererUI.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Velvet/Core/Application.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	struct RendererUIStorage
	{
		Ref<VertexArray> ButtonVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Scope<RendererUIStorage> s_UIData;

	void RendererUI::Init()
	{
		VL_PROFILE_FUNCTION();

		s_UIData = CreateScope<RendererUIStorage>();
		s_UIData->ButtonVertexArray = VertexArray::Create();

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

		s_UIData->ButtonVertexArray->AddVertexBuffer(squareVBO);
		s_UIData->ButtonVertexArray->SetIndexBuffer(squareIBO);
		s_UIData->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void RendererUI::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		s_UIData.release();
	}

	void RendererUI::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetProjectionMatrix());
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.4f, 0.4f, 0.0f });
		//std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->UploadUniformMat4("u_Transform", transform);
	}

	void RendererUI::EndScene()
	{
		VL_PROFILE_FUNCTION();
	}

	void RendererUI::DrawButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color)
	{
		VL_PROFILE_FUNCTION();

		glm::vec2 normalizedPos = PixelsToNDC(pixelPosition);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { normalizedPos.x, normalizedPos.y, 0.0f });

		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->UploadUniformFloat4("u_Color", color);
		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->UploadUniformMat4("u_Transform", transform);

		s_UIData->ButtonVertexArray->Bind();

		RenderCommand::DrawIndexed(s_UIData->ButtonVertexArray);
	}

	glm::vec2 RendererUI::PixelsToNDC(const glm::vec2& pixelPosition)
	{
		VL_PROFILE_FUNCTION();

		Application& app = Application::Get();
		float windowWidth = (float)app.GetWindow().GetWidth();
		float windowHeight = (float)app.GetWindow().GetHeight();
		float aspectRatio = windowWidth / windowHeight;
		float normalizedX, normalizedY;

		VL_CORE_TRACE("Aspect Ratio: {} / {}", windowWidth, windowHeight);
		VL_CORE_TRACE("Aspect Ratio: {}", aspectRatio);

		normalizedX = (2.0f * pixelPosition.x) / windowWidth - 1.0f;
		normalizedY = 1.0f - (2.0f * pixelPosition.y) / windowHeight;

		return glm::vec2(normalizedX * aspectRatio, normalizedY);
	}

}