#include <vlpch.h>
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Velvet {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}