#include <vlpch.h>
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Velvet {

	Velvet::VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		VL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		VL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}