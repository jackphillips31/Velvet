#include <vlpch.h>
#include "Velvet/Renderer/UIController.h"

#include "Velvet/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Velvet {

	struct UIData
	{
		Scope<BatchBuffer> BatchBufferController;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		glm::vec2 InitialWindowDimensions;
		uint32_t IDCounter = 0;
		std::vector<UIElement*> UIElements;
	};

	static UIData UIControllerData;

	OrthographicCameraController UIController::m_UICameraController = OrthographicCameraController();

	void UIController::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing UIController");

		Window& tempWindowRef = Application::Get().GetWindow();
		glm::vec2 windowDimensions(tempWindowRef.GetWidth(), tempWindowRef.GetHeight());

		OrthographicCameraController::CameraSettings UICameraSettings;
		UICameraSettings.rotation = false;
		UICameraSettings.zoom = false;

		m_UICameraController.SetCameraSettings(UICameraSettings);
		m_UICameraController.SetInitialWindowDimensions(windowDimensions);

		UIControllerData.BatchBufferController = BatchBuffer::Create(BatchType::Quad);

		UIControllerData.TextureShader = Renderer::GetShaderLibrary().Get("Texture");
		UIControllerData.WhiteTexture = Renderer::GetTexture2DLibrary().Get("DefaultWhite");

		UIControllerData.TextureShader->Bind();
		UIControllerData.TextureShader->SetInt("u_Texture", 0);
	}

	void UIController::Shutdown()
	{
		VL_PROFILE_FUNCTION();

		ClearUIElements();
		UIControllerData.BatchBufferController.reset();
		UIControllerData.TextureShader.reset();
		UIControllerData.WhiteTexture.reset();
	}

	void UIController::OnEvent(Event& e)
	{
		m_UICameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(VL_STATIC_BIND_EVENT_FN(UIController::OnMouseButtonPressed));
	}

	void UIController::AddButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation)
	{
		glm::vec3 normPos = { NDCFromPixel(pixelPosition, orientation), 1.0f };
		AddQuad(normPos, size, color);
	}

	void UIController::AddQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		VL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		UIControllerData.BatchBufferController->AddQuad(transform, color, 12);
		AddElement(position, size);
	}

	void UIController::BeginScene()
	{
		VL_PROFILE_FUNCTION();

		UIControllerData.IDCounter = 0;
		ClearUIElements();

		UIControllerData.BatchBufferController->StartBatch();

		UIControllerData.TextureShader->Bind();
		UIControllerData.TextureShader->SetMat4("u_ViewProjection", m_UICameraController.GetCamera().GetProjectionMatrix());
	}

	void UIController::EndScene()
	{
		VL_PROFILE_FUNCTION();

		UIControllerData.BatchBufferController->Flush();
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
		Window& tempWindowRef = Application::Get().GetWindow();
		glm::vec2 windowDimensions = glm::vec2(tempWindowRef.GetWidth(), tempWindowRef.GetHeight());
		glm::vec2 orientationFactors = GetOrientationFactors(orientation);
		float aspectRatio = windowDimensions.x / windowDimensions.y;

		float scale = m_UICameraController.GetScale();

		float normalizedX = (2.0f * pixelPosition.x) / windowDimensions.x + orientationFactors.x;
		float normalizedY = orientationFactors.y - (2.0f * pixelPosition.y) / windowDimensions.y;

		return glm::vec2(normalizedX * aspectRatio * scale, normalizedY * scale);
	}

	void UIController::AddElement(const glm::vec2& position, const glm::vec2& size)
	{
		VL_PROFILE_FUNCTION();

		float xFactor = size.x / 2;
		float yFactor = size.y / 2;

		UIElement* newElement = new UIElement;
		newElement->x.x = position.x - xFactor;
		newElement->x.y = position.x + xFactor;
		newElement->y.x = position.y - yFactor;
		newElement->y.y = position.y + yFactor;
		newElement->ElementID = UIControllerData.IDCounter;

		UIControllerData.UIElements.push_back(newElement);
		UIControllerData.IDCounter++;
	}

	bool UIController::CheckElementHover(const glm::vec2& mousePosition)
	{
		glm::vec2 newPos = NDCFromPixel(mousePosition);
		for (UIElement* ptr : UIControllerData.UIElements)
		{
			bool isBetween = (newPos.x >= ptr->x.x && newPos.x <= ptr->x.y && newPos.y >= ptr->y.x && newPos.y <= ptr->y.y);
			if (isBetween)
				return isBetween;
		}
		return false;
	}

	void UIController::ClearUIElements()
	{
		VL_PROFILE_FUNCTION();

		for (size_t i = UIControllerData.UIElements.size(); i > 0; i--)
		{
			auto it = UIControllerData.UIElements.begin() + i - 1;
			delete* it;
			UIControllerData.UIElements.erase(it);
		}
		UIControllerData.UIElements.clear();
	}

	bool UIController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		std::pair<float, float> mousePosition = Input::GetMousePosition();

		bool result = CheckElementHover({ mousePosition.first, mousePosition.second });
		if (result && e.GetMouseButton() == VL_MOUSE_BUTTON_0)
			VL_CORE_INFO("Element Clicked!");

		return false;
	}

}