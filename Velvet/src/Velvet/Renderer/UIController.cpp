#include <vlpch.h>
#include "Velvet/Renderer/UIController.h"

#include "Velvet/Renderer/Renderer.h"

namespace Velvet {

	Scope<UIController::UIData> UIController::m_UIData = nullptr;
	Scope<std::vector<UIController::UIElement>> UIController::m_UIElements = nullptr;

	void UIController::Init()
	{
		VL_PROFILE_FUNCTION();
		VL_CORE_INFO("Initializing UIController");

		Window& tempWindowRef = Application::Get().GetWindow();
		glm::vec2 windowDimensions(tempWindowRef.GetWidth(), tempWindowRef.GetHeight());

		m_UIData = CreateScope<UIData>(tempWindowRef, windowDimensions);
		m_UIElements = CreateScope<std::vector<UIElement>>();

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

		m_UIData.reset();
		m_UIElements.reset();
	}

	void UIController::OnEvent(Event& e)
	{
		m_UIData->UICameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(VL_STATIC_BIND_EVENT_FN(UIController::OnWindowResize));
	}

	void UIController::AddElement(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation)
	{
		VL_PROFILE_FUNCTION();

		UIElement newElement(pixelPosition, size, color, orientation);
		m_UIElements->push_back(newElement);

		glm::vec3 normPos = { NDCFromPixel(pixelPosition, orientation), 1.0f };

		Ref<QuadBufferElement> quadElement = CreateRef<QuadBufferElement>(normPos, color);
		m_UIData->UIRenderBatch.AddQuadBufferElement(quadElement);
	}

	void UIController::PreRender()
	{
		VL_PROFILE_FUNCTION();

		m_UIData->UIRenderBatch.CreateBuffers();

		m_UIData->UIRenderBatch.GetVertexBufferObject()->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord"},
			{ ShaderDataType::Float4, "a_Color" }
			});

		m_UIData->BatchVertexArray = VertexArray::Create();
		m_UIData->BatchVertexArray->AddVertexBuffer(m_UIData->UIRenderBatch.GetVertexBufferObject());
		m_UIData->BatchVertexArray->SetIndexBuffer(m_UIData->UIRenderBatch.GetIndexBufferObject());
	}

	void UIController::Render()
	{
		VL_PROFILE_FUNCTION();

		BeginScene(m_UIData->UICameraController.GetCamera());

		glm::mat4 transform = glm::mat4(1.0f);

		m_UIData->TextureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
		m_UIData->TextureShader->SetMat4("u_Transform", transform);
		m_UIData->WhiteTexture->Bind();

		m_UIData->BatchVertexArray->Bind();
		RenderCommand::DrawIndexed(m_UIData->BatchVertexArray);

		EndScene();
	}

	void UIController::BeginScene(const OrthographicCamera& camera)
	{
		VL_PROFILE_FUNCTION();

		m_UIData->TextureShader->Bind();
		m_UIData->TextureShader->SetMat4("u_ViewProjection", camera.GetProjectionMatrix());
	}

	void UIController::EndScene()
	{
		VL_PROFILE_FUNCTION();
	}

	bool UIController::OnWindowResize(WindowResizeEvent& e)
	{
		uint32_t index = 0;
		for (Ref<QuadBufferElement>& element : *m_UIData->QuadElementBuffer)
		{
			VL_CORE_WARN("---------------- QUAD {} ----------------", index);

			index++;
		}

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

	UIController::UIData::UIData(Window& window, glm::vec2& windowDimensions)
		: WindowRef(window), UICameraController(windowDimensions), InitialWindowDimensions(windowDimensions), QuadElementBuffer(CreateRef<std::vector<Ref<QuadBufferElement>>>()), UIRenderBatch(QuadElementBuffer)
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