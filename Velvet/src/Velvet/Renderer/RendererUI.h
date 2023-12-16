#pragma once

#include "Renderer2D.h"
#include "OrthographicCamera.h"

namespace Velvet {

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

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color, const Orientation& orientation);
	private:
		static glm::vec2& GetWindowDimensions();
		static glm::vec2& GetOrientationFactors(const Orientation& orientation);
		static glm::vec2& NDCFromPixel(const glm::vec2& pixelPosition, const Orientation& orientation);
	};

}