#include <vlpch.h>
#include "Velvet/Renderer/GraphicsContext.h"

#include "Velvet/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Velvet {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	VL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		VL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}