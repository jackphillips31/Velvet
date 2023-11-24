#pragma once

#include "RenderCommand.h"

namespace Velvet {

	class Renderer
	{
	public:
		static void BeginScene();	// TODO
		static void EndScene();		// TODO

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}