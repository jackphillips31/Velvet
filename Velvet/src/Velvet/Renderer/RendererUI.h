#pragma once

#include "Velvet/Core/Application.h"

#include "Velvet/Renderer/OrthographicCameraController.h"
#include "Velvet/Renderer/Texture.h"

namespace Velvet {

	struct UIElement
	{
		glm::vec2 x;
		glm::vec2 y;
		uint32_t ElementID;
	};

	class RendererUI
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

		static void BeginScene();
		static void EndScene();

		// UI Primitives
		static void AddButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);
		static void AddButton(const glm::vec2& pixelPosition, const glm::vec2& size, const Ref<Texture2D>& texture, const Orientation& orientation);
		
		// Primitives
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	private:
		static glm::vec2 GetOrientationFactors(const Orientation& orientation);
		static glm::vec2 NDCFromPixel(const glm::vec2& pixelPosition, const Orientation& orientation = Orientation::TopLeft);

		static void AddElement(const glm::vec2& position, const glm::vec2& size);
		static bool CheckElementHover(const glm::vec2& mousePosition);
		static void ClearUIElements();
		
		static bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	private:
		static OrthographicCameraController m_UICameraController;
	};

}