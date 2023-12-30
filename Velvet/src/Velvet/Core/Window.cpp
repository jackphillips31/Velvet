#include <vlpch.h>
#include "Velvet/Core/Window.h"

#ifdef VL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Velvet {

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef VL_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		VL_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

}