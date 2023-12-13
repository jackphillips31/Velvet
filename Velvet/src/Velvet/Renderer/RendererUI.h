#pragma once

#include "Renderer2D.h"
#include "OrthographicCamera.h"

namespace Velvet {

	class RendererUI
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawButton(const glm::vec2& pixelPosition, const glm::vec2& size, const glm::vec4& color);
	private:
		static glm::vec2 PixelsToNDC(const glm::vec2& pixelPosition);
	};

}