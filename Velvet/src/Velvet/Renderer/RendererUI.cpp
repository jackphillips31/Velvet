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
	}

	void RendererUI::EndScene()
	{
		VL_PROFILE_FUNCTION();
	}

	void RendererUI::DrawButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const RendererUI::Orientation orientation)
	{
		VL_PROFILE_FUNCTION();

		glm::vec2 normalizedPos = NDCFromPixel(pixelPosition, orientation);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { normalizedPos.x, normalizedPos.y, 0.0f });

		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->UploadUniformFloat4("u_Color", color);
		std::dynamic_pointer_cast<OpenGLShader>(s_UIData->FlatColorShader)->UploadUniformMat4("u_Transform", transform);

		s_UIData->ButtonVertexArray->Bind();

		RenderCommand::DrawIndexed(s_UIData->ButtonVertexArray);
	}

	glm::vec2 RendererUI::NDCFromTopLeft(const glm::vec2& pixelPosition)
	{
		VL_PROFILE_FUNCTION();

		// TODO: figure out how to have this NOT hardcoded!
		float initialHeight = 720.0f;

		glm::vec2& windowDimensions = GetWindowDimensions();
		float aspectRatio = windowDimensions.x / windowDimensions.y;
		float normalizedX, normalizedY;

		float scale = windowDimensions.y / initialHeight;

		normalizedX = (2.0f * pixelPosition.x) / windowDimensions.x - 1.0f;
		normalizedY = 1.0f - (2.0f * pixelPosition.y) / windowDimensions.y;

		return glm::vec2(normalizedX * aspectRatio * scale, normalizedY * scale);
	}

	glm::vec2 RendererUI::NDCFromTopRight(const glm::vec2& pixelPosition)
	{
		VL_PROFILE_FUNCTION();

		// TODO: figure out how to have this NOT hardcoded!
		float initialHeight = 720.0f;

		glm::vec2& windowDimensions = GetWindowDimensions();
		float aspectRatio = windowDimensions.x / windowDimensions.y;
		float normalizedX, normalizedY;

		float scale = windowDimensions.y / initialHeight;

		normalizedX = (2.0f * pixelPosition.x) / windowDimensions.x + 1.0f;
		normalizedY = 1.0f - (2.0f * pixelPosition.y) / windowDimensions.y;

		return glm::vec2(normalizedX * aspectRatio * scale, normalizedY * scale);
	}

	glm::vec2 RendererUI::NDCFromPixel(const glm::vec2& pixelPosition, const Orientation orientation)
	{
		VL_PROFILE_FUNCTION();

		// TODO: figure out how to have this NOT hardcoded!
		float initialHeight = 720.0f;

		float xFactor = 0.0f;
		if (orientation == TopLeft || orientation == BottomLeft)
			xFactor = -1.0f;
		else if(orientation == TopRight || orientation == BottomRight)
			xFactor = 1.0f;

		float yFactor = 0.0f;
		if (orientation == TopLeft || orientation == TopRight)
			yFactor = 1.0f;
		else if (orientation == BottomLeft || orientation == BottomRight)
			yFactor = -1.0f;

		glm::vec2& windowDimensions = GetWindowDimensions();
		float aspectRatio = windowDimensions.x / windowDimensions.y;
		float normalizedX, normalizedY;

		float scale = windowDimensions.y / initialHeight;

		normalizedX = (2.0f * pixelPosition.x) / windowDimensions.x + xFactor;
		normalizedY = yFactor - (2.0f * pixelPosition.y) / windowDimensions.y;

		return glm::vec2(normalizedX * aspectRatio * scale, normalizedY * scale);
	}

	glm::vec2 RendererUI::GetWindowDimensions()
	{
		Application& app = Application::Get();
		return glm::vec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
	}

}