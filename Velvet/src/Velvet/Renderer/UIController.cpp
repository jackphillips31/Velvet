#include <vlpch.h>
#include "UIController.h"

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	Scope<UIController::UIData> UIController::m_UIData = nullptr;
	Scope<std::vector<UIController::UIElement>> UIController::m_UIElements = nullptr;

	void UIController::Init()
	{
		VL_PROFILE_FUNCTION();

		VL_CORE_INFO("UIController Init");

		Window& tempWindowRef = Application::Get().GetWindow();
		glm::vec2 windowDimensions(tempWindowRef.GetWidth(), tempWindowRef.GetHeight());

		m_UIData = CreateScope<UIData>(tempWindowRef, windowDimensions);
		m_UIElements = CreateScope<std::vector<UIElement>>();

		OrthographicCameraController::CameraSettings UICameraSettings;
		UICameraSettings.rotation = false;
		UICameraSettings.zoom = false;

		m_UIData->UICameraController.SetCameraSettings(UICameraSettings);

		m_UIData->QuadVertexArray = VertexArray::Create();

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

		m_UIData->QuadVertexArray->AddVertexBuffer(squareVBO);
		m_UIData->QuadVertexArray->SetIndexBuffer(squareIBO);

		m_UIData->FlatColorShader = Renderer::GetShaderLibrary().Get("FlatColor");
	}

	void UIController::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		m_UIData.release();
	}

	void UIController::AddElement(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation)
	{
		UIElement newElement(pixelPosition, size, color, orientation);
		m_UIElements->push_back(newElement);
	}

	void UIController::Render()
	{
		VL_PROFILE_FUNCTION();

		BeginScene(m_UIData->UICameraController.GetCamera());
		if (m_UIElements->empty())
		{
			VL_CORE_ASSERT(false, "No UI Elements to render!");
		}
		else
		{
			for (UIElement& elementRef : *m_UIElements)
			{
				//VL_CORE_INFO("Render Element!");
				DrawButton(elementRef.PixelPosition, elementRef.Size, elementRef.Color, elementRef.ElementOrientation);
			}
		}
		EndScene();
	}

	void UIController::OnEvent(Event& e)
	{
		m_UIData->UICameraController.OnEvent(e);
	}

	void UIController::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		m_UIData->FlatColorShader->Bind();
		m_UIData->FlatColorShader->SetMat4("u_ViewProjection", camera.GetProjectionMatrix());
	}

	void UIController::EndScene()
	{
		VL_PROFILE_FUNCTION();
	}

	void UIController::DrawButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation)
	{
		VL_PROFILE_FUNCTION();

		glm::vec2 normalizedPos = NDCFromPixel(pixelPosition, orientation);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { normalizedPos.x, normalizedPos.y, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		m_UIData->FlatColorShader->Bind();
		m_UIData->FlatColorShader->SetFloat4("u_Color", color);
		m_UIData->FlatColorShader->SetMat4("u_Transform", transform);

		m_UIData->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(m_UIData->QuadVertexArray);
	}

	glm::vec2 UIController::GetWindowDimensions()
	{
		return glm::vec2(m_UIData->WindowRef.GetWidth(), m_UIData->WindowRef.GetHeight());
	}

	glm::vec2 UIController::GetOrientationFactors(const Orientation& orientation)
	{
		float xFactor = 0.0f;
		if (orientation == TopLeft || orientation == BottomLeft)
			xFactor = -1.0f;
		else if (orientation == TopRight || orientation == BottomRight)
			xFactor = 1.0f;

		float yFactor = 0.0f;
		if (orientation == TopLeft || orientation == TopRight)
			yFactor = 1.0f;
		else if (orientation == BottomLeft || orientation == BottomRight)
			yFactor = -1.0f;

		return glm::vec2(xFactor, yFactor);
	}

	glm::vec2 UIController::NDCFromPixel(const glm::vec2& pixelPosition, const Orientation& orientation)
	{
		VL_PROFILE_FUNCTION();

		glm::vec2& windowDimensions = GetWindowDimensions();
		glm::vec2& orientationFactors = GetOrientationFactors(orientation);
		float aspectRatio = windowDimensions.x / windowDimensions.y;

		float scale = windowDimensions.y / m_UIData->InitialWindowDimensions.y;

		float normalizedX = (2.0f * pixelPosition.x) / windowDimensions.x + orientationFactors.x;
		float normalizedY = orientationFactors.y - (2.0f * pixelPosition.y) / windowDimensions.y;

		return glm::vec2(normalizedX * aspectRatio * scale, normalizedY * scale);
	}

	UIController::UIData::UIData(Window& window, glm::vec2& windowDimensions)
		: WindowRef(window), UICameraController(windowDimensions), InitialWindowDimensions(windowDimensions)
	{
	}

	UIController::UIElement::UIElement(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation)
	{
		PixelPosition = pixelPosition;
		Size = size;
		Color = color;
		ElementOrientation = orientation;
	}

}