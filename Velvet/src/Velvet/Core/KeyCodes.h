#pragma once

namespace Velvet {

	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

}

#define VL_KEY_SPACE           ::Velvet::Key::Space
#define VL_KEY_APOSTROPHE      ::Velvet::Key::Apostrophe    /* ' */
#define VL_KEY_COMMA           ::Velvet::Key::Comma         /* , */
#define VL_KEY_MINUS           ::Velvet::Key::Minus         /* - */
#define VL_KEY_PERIOD          ::Velvet::Key::Period        /* . */
#define VL_KEY_SLASH           ::Velvet::Key::Slash         /* / */
#define VL_KEY_0               ::Velvet::Key::D0
#define VL_KEY_1               ::Velvet::Key::D1
#define VL_KEY_2               ::Velvet::Key::D2
#define VL_KEY_3               ::Velvet::Key::D3
#define VL_KEY_4               ::Velvet::Key::D4
#define VL_KEY_5               ::Velvet::Key::D5
#define VL_KEY_6               ::Velvet::Key::D6
#define VL_KEY_7               ::Velvet::Key::D7
#define VL_KEY_8               ::Velvet::Key::D8
#define VL_KEY_9               ::Velvet::Key::D9
#define VL_KEY_SEMICOLON       ::Velvet::Key::Semicolon     /* ; */
#define VL_KEY_EQUAL           ::Velvet::Key::Equal         /* = */
#define VL_KEY_A               ::Velvet::Key::A
#define VL_KEY_B               ::Velvet::Key::B
#define VL_KEY_C               ::Velvet::Key::C
#define VL_KEY_D               ::Velvet::Key::D
#define VL_KEY_E               ::Velvet::Key::E
#define VL_KEY_F               ::Velvet::Key::F
#define VL_KEY_G               ::Velvet::Key::G
#define VL_KEY_H               ::Velvet::Key::H
#define VL_KEY_I               ::Velvet::Key::I
#define VL_KEY_J               ::Velvet::Key::J
#define VL_KEY_K               ::Velvet::Key::K
#define VL_KEY_L               ::Velvet::Key::L
#define VL_KEY_M               ::Velvet::Key::M
#define VL_KEY_N               ::Velvet::Key::N
#define VL_KEY_O               ::Velvet::Key::O
#define VL_KEY_P               ::Velvet::Key::P
#define VL_KEY_Q               ::Velvet::Key::Q
#define VL_KEY_R               ::Velvet::Key::R
#define VL_KEY_S               ::Velvet::Key::S
#define VL_KEY_T               ::Velvet::Key::T
#define VL_KEY_U               ::Velvet::Key::U
#define VL_KEY_V               ::Velvet::Key::V
#define VL_KEY_W               ::Velvet::Key::W
#define VL_KEY_X               ::Velvet::Key::X
#define VL_KEY_Y               ::Velvet::Key::Y
#define VL_KEY_Z               ::Velvet::Key::Z
#define VL_KEY_LEFT_BRACKET    ::Velvet::Key::LeftBracket   /* [ */
#define VL_KEY_BACKSLASH       ::Velvet::Key::Backslash     /* \ */
#define VL_KEY_RIGHT_BRACKET   ::Velvet::Key::RightBracket  /* ] */
#define VL_KEY_GRAVE_ACCENT    ::Velvet::Key::GraveAccent   /* ` */
#define VL_KEY_WORLD_1         ::Velvet::Key::World1        /* non-US #1 */
#define VL_KEY_WORLD_2         ::Velvet::Key::World2        /* non-US #2 */

#define VL_KEY_ESCAPE          ::Velvet::Key::Escape
#define VL_KEY_ENTER           ::Velvet::Key::Enter
#define VL_KEY_TAB             ::Velvet::Key::Tab
#define VL_KEY_BACKSPACE       ::Velvet::Key::Backspace
#define VL_KEY_INSERT          ::Velvet::Key::Insert
#define VL_KEY_DELETE          ::Velvet::Key::Delete
#define VL_KEY_RIGHT           ::Velvet::Key::Right
#define VL_KEY_LEFT            ::Velvet::Key::Left
#define VL_KEY_DOWN            ::Velvet::Key::Down
#define VL_KEY_UP              ::Velvet::Key::Up
#define VL_KEY_PAGE_UP         ::Velvet::Key::PageUp
#define VL_KEY_PAGE_DOWN       ::Velvet::Key::PageDown
#define VL_KEY_HOME            ::Velvet::Key::Home
#define VL_KEY_END             ::Velvet::Key::End
#define VL_KEY_CAPS_LOCK       ::Velvet::Key::CapsLock
#define VL_KEY_SCROLL_LOCK     ::Velvet::Key::ScrollLock
#define VL_KEY_NUM_LOCK        ::Velvet::Key::NumLock
#define VL_KEY_PRINT_SCREEN    ::Velvet::Key::PrintScreen
#define VL_KEY_PAUSE           ::Velvet::Key::Pause
#define VL_KEY_F1              ::Velvet::Key::F1
#define VL_KEY_F2              ::Velvet::Key::F2
#define VL_KEY_F3              ::Velvet::Key::F3
#define VL_KEY_F4              ::Velvet::Key::F4
#define VL_KEY_F5              ::Velvet::Key::F5
#define VL_KEY_F6              ::Velvet::Key::F6
#define VL_KEY_F7              ::Velvet::Key::F7
#define VL_KEY_F8              ::Velvet::Key::F8
#define VL_KEY_F9              ::Velvet::Key::F9
#define VL_KEY_F10             ::Velvet::Key::F10
#define VL_KEY_F11             ::Velvet::Key::F11
#define VL_KEY_F12             ::Velvet::Key::F12
#define VL_KEY_F13             ::Velvet::Key::F13
#define VL_KEY_F14             ::Velvet::Key::F14
#define VL_KEY_F15             ::Velvet::Key::F15
#define VL_KEY_F16             ::Velvet::Key::F16
#define VL_KEY_F17             ::Velvet::Key::F17
#define VL_KEY_F18             ::Velvet::Key::F18
#define VL_KEY_F19             ::Velvet::Key::F19
#define VL_KEY_F20             ::Velvet::Key::F20
#define VL_KEY_F21             ::Velvet::Key::F21
#define VL_KEY_F22             ::Velvet::Key::F22
#define VL_KEY_F23             ::Velvet::Key::F23
#define VL_KEY_F24             ::Velvet::Key::F24
#define VL_KEY_F25             ::Velvet::Key::F25

/* Keypad */
#define VL_KEY_KP_0            ::Velvet::Key::KP0
#define VL_KEY_KP_1            ::Velvet::Key::KP1
#define VL_KEY_KP_2            ::Velvet::Key::KP2
#define VL_KEY_KP_3            ::Velvet::Key::KP3
#define VL_KEY_KP_4            ::Velvet::Key::KP4
#define VL_KEY_KP_5            ::Velvet::Key::KP5
#define VL_KEY_KP_6            ::Velvet::Key::KP6
#define VL_KEY_KP_7            ::Velvet::Key::KP7
#define VL_KEY_KP_8            ::Velvet::Key::KP8
#define VL_KEY_KP_9            ::Velvet::Key::KP9
#define VL_KEY_KP_DECIMAL      ::Velvet::Key::KPDecimal
#define VL_KEY_KP_DIVIDE       ::Velvet::Key::KPDivide
#define VL_KEY_KP_MULTIPLY     ::Velvet::Key::KPMultiply
#define VL_KEY_KP_SUBTRACT     ::Velvet::Key::KPSubtract
#define VL_KEY_KP_ADD          ::Velvet::Key::KPAdd
#define VL_KEY_KP_ENTER        ::Velvet::Key::KPEnter
#define VL_KEY_KP_EQUAL        ::Velvet::Key::KPEqual

#define VL_KEY_LEFT_SHIFT      ::Velvet::Key::LeftShift
#define VL_KEY_LEFT_CONTROL    ::Velvet::Key::LeftControl
#define VL_KEY_LEFT_ALT        ::Velvet::Key::LeftAlt
#define VL_KEY_LEFT_SUPER      ::Velvet::Key::LeftSuper
#define VL_KEY_RIGHT_SHIFT     ::Velvet::Key::RightShift
#define VL_KEY_RIGHT_CONTROL   ::Velvet::Key::RightControl
#define VL_KEY_RIGHT_ALT       ::Velvet::Key::RightAlt
#define VL_KEY_RIGHT_SUPER     ::Velvet::Key::RightSuper
#define VL_KEY_MENU            ::Velvet::Key::Menu