#include <vlpch.h>
#include "Velvet/Core/Input.h"

#ifdef VL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Velvet {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
	#ifdef VL_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
	#else
		VL_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

}