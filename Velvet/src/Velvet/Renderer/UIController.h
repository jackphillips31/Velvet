#pragma once

#include "Velvet/Core/Application.h"

#include "OrthographicCameraController.h"
#include "Shader.h"
#include "VertexArray.h"

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

		static void AddElement(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);
		static void Render();
		static void OnEvent(Event& e);
	private:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void DrawButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);

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
			Ref<VertexArray> QuadVertexArray;
			Ref<Shader> FlatColorShader;
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