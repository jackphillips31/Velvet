#pragma once

#include "Velvet/Core/Application.h"

#include "Velvet/Renderer/Batch/BatchBuffer.h"
#include "Velvet/Renderer/OrthographicCameraController.h"
#include "Velvet/Renderer/Shader.h"
#include "Velvet/Renderer/Texture.h"

namespace Velvet {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
	};

	struct UIElement
	{
		glm::vec2 x;
		glm::vec2 y;
		uint32_t ElementID;
	};

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

		static void AddButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);
		static void AddQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void BeginScene();
		static void EndScene();
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