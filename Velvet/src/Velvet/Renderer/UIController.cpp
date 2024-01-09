#include <vlpch.h>
#include "Velvet/Renderer/UIController.h"

#include "Velvet/Renderer/BatchBuffer.h"
#include "Velvet/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	Scope<UIController::UIData> UIController::m_UIData = nullptr;

	void UIController::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing UIController");

		Window& tempWindowRef = Application::Get().GetWindow();
		glm::vec2 windowDimensions(tempWindowRef.GetWidth(), tempWindowRef.GetHeight());

		m_UIData = CreateScope<UIData>(tempWindowRef, windowDimensions);

		OrthographicCameraController::CameraSettings UICameraSettings;
		UICameraSettings.rotation = false;
		UICameraSettings.zoom = false;

		m_UIData->UICameraController.SetCameraSettings(UICameraSettings);

		m_UIData->TextureShader = Renderer::GetShaderLibrary().Get("Texture");
		m_UIData->WhiteTexture = Renderer::GetTexture2DLibrary().Get("DefaultWhite");

		m_UIData->TextureShader->Bind();
		m_UIData->TextureShader->SetInt("u_Texture", 0);
	}

	void UIController::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		for (size_t i = m_UIData->UIElements.size(); i > 0; i--)
		{
			auto it = m_UIData->UIElements.begin() + i - 1;
			delete* it;
			m_UIData->UIElements.erase(it);
		}
		m_UIData->UIElements.clear();

		m_UIData.reset();
	}

	void UIController::OnEvent(Event& e)
	{
		m_UIData->UICameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(VL_STATIC_BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<MouseMovedEvent>(VL_STATIC_BIND_EVENT_FN(OnMouseMoved));
	}

	void UIController::AddButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation)
	{
		glm::vec3 normPos = { NDCFromPixel(pixelPosition, orientation), 1.0f };
		AddQuad(normPos, size, color);
	}

	void UIController::AddQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		VL_PROFILE_FUNCTION();

		glm::vec2 pixelPos = PixelFromNDC({ position.x, position.y });
		AddElement(pixelPos, size);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		BatchBuffer::AddQuad(transform, color, 12);
	}

	void UIController::BeginScene()
	{
		VL_PROFILE_FUNCTION();

		BatchBuffer::StartBatch();

		m_UIData->TextureShader->Bind();
		m_UIData->TextureShader->SetMat4("u_ViewProjection", m_UIData->UICameraController.GetCamera().GetProjectionMatrix());
	}

	void UIController::EndScene()
	{
		VL_PROFILE_FUNCTION();

		BatchBuffer::Flush();
	}

	bool UIController::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}

	bool UIController::OnMouseMoved(MouseMovedEvent& e)
	{
		VL_CORE_WARN(e);
		bool hover = CheckElementHover(glm::vec2(e.GetX(), e.GetY()));
		VL_CORE_WARN("{}", hover);
		return false;
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
		glm::vec2& windowDimensions = GetWindowDimensions();
		glm::vec2& orientationFactors = GetOrientationFactors(orientation);
		float aspectRatio = windowDimensions.x / windowDimensions.y;

		float scale = windowDimensions.y / m_UIData->InitialWindowDimensions.y;

		float normalizedX = (2.0f * pixelPosition.x) / windowDimensions.x + orientationFactors.x;
		float normalizedY = orientationFactors.y - (2.0f * pixelPosition.y) / windowDimensions.y;

		return glm::vec2(normalizedX * aspectRatio * scale, normalizedY * scale);
	}

	glm::vec2 UIController::PixelFromNDC(const glm::vec2& position)
	{
		glm::vec2& windowDimensions = GetWindowDimensions();
		float aspectRatio = windowDimensions.x / windowDimensions.y;
		float scale = windowDimensions.y / m_UIData->InitialWindowDimensions.y;

		float pixelPositionX = (((position.x / (aspectRatio * scale)) + 1) * windowDimensions.x) / 2;
		float pixelPositionY = ((position.y - 1) * windowDimensions.y) / -2;

		return glm::vec2(pixelPositionX, pixelPositionY);
	}

	float UIController::PixelPerNDC()
	{
		float height = m_UIData->WindowRef.GetHeight();
		float scale = 2 * m_UIData->UICameraController.GetScale();
		return height / scale;
	}

	void UIController::AddElement(const glm::vec2& position, const glm::vec2& size)
	{
		float pxPerNDC = PixelPerNDC();
		float xFactor = size.x / 2;
		float yFactor = size.y / 2;

		UIElement* newElement = new UIElement;
		newElement->x.x = position.x - pxPerNDC * xFactor;
		newElement->x.y = position.x + pxPerNDC * xFactor;
		newElement->y.x = position.y - pxPerNDC * yFactor;
		newElement->y.y = position.y + pxPerNDC * yFactor;
		newElement->ElementID = m_UIData->IDCounter;

		m_UIData->UIElements.push_back(newElement);
		m_UIData->IDCounter++;
	}

	bool UIController::CheckElementHover(const glm::vec2& mousePosition)
	{
		for (UIElement* ptr : m_UIData->UIElements)
		{
			bool isBetween = (mousePosition.x >= ptr->x.x && mousePosition.x <= ptr->x.y && mousePosition.y >= ptr->y.x && mousePosition.y <= ptr->y.y);
			if (isBetween)
				return isBetween;
		}
		return false;
	}

	UIController::UIData::UIData(Window& window, glm::vec2& windowDimensions)
		: WindowRef(window), UICameraController(windowDimensions), InitialWindowDimensions(windowDimensions)
	{
	}

}