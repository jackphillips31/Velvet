#include <vlpch.h>
#include "Input.h"

#ifdef VL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Velvet {

	void Input::Init()
	{
	#ifdef VL_PLATFORM_WINDOWS
		s_Instance = CreateScope<WindowsInput>();
	#else
		VL_CORE_ASSERT(false, "Unknown Platform!");
		s_Instance = nullptr;
	#endif
	}

}