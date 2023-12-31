#pragma once

#include "Velvet/Core/Application.h"

#include "Velvet/Renderer/Batch.h"
#include "Velvet/Renderer/OrthographicCameraController.h"
#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/Texture.h"
#include "Velvet/Renderer/VertexArray.h"

namespace Velvet {

	class UIController
	{
	public:
		enum Orientation
		{
			None = 0,
			TopLeft,
			TopRight,
			BottomLeft,
			BottomRight
		};
	public:
		static void Init();
		static void Shutdown();
		static void OnEvent(Event& e);

		static void AddElement(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);
		static void PreRender();
		static void Render();
	private:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static bool OnWindowResize(WindowResizeEvent& e);

		static glm::vec2 GetWindowDimensions();
		static glm::vec2 GetOrientationFactors(const Orientation& orientation);
		static glm::vec2 NDCFromPixel(const glm::vec2& pixelPosition, const Orientation& orientation);
	private:
		struct UIData
		{
			UIData(Window& window, glm::vec2& windowDimensions);

			Window& WindowRef;
			OrthographicCameraController UICameraController;
			glm::vec2 InitialWindowDimensions;
			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;

			Ref<VertexArray> BatchVertexArray;
			Ref<std::vector<Ref<QuadBufferElement>>> QuadElementBuffer;
			RenderBatch UIRenderBatch;
		};

		struct UIElement
		{
			UIElement(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);

			glm::vec2 PixelPosition;
			glm::vec2 Size;
			glm::vec4 Color;
			Orientation ElementOrientation;
		};

		static Scope<UIData> m_UIData;
		static Scope<std::vector<UIElement>> m_UIElements;
	};

}