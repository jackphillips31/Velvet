#pragma once

namespace Velvet {

	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

}

#define VL_MOUSE_BUTTON_0      ::Velvet::Mouse::Button0
#define VL_MOUSE_BUTTON_1      ::Velvet::Mouse::Button1
#define VL_MOUSE_BUTTON_2      ::Velvet::Mouse::Button2
#define VL_MOUSE_BUTTON_3      ::Velvet::Mouse::Button3
#define VL_MOUSE_BUTTON_4      ::Velvet::Mouse::Button4
#define VL_MOUSE_BUTTON_5      ::Velvet::Mouse::Button5
#define VL_MOUSE_BUTTON_6      ::Velvet::Mouse::Button6
#define VL_MOUSE_BUTTON_7      ::Velvet::Mouse::Button7
#define VL_MOUSE_BUTTON_LAST   ::Velvet::Mouse::ButtonLast
#define VL_MOUSE_BUTTON_LEFT   ::Velvet::Mouse::ButtonLeft
#define VL_MOUSE_BUTTON_RIGHT  ::Velvet::Mouse::ButtonRight
#define VL_MOUSE_BUTTON_MIDDLE ::Velvet::Mouse::ButtonMiddle