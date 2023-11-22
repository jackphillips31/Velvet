#include <vlpch.h>
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Velvet {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VL_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		VL_CORE_INFO("Initializing OpenGL Context");
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VL_CORE_ASSERT(status, "Failed to initialize GLAD!");

		VL_CORE_INFO("OpenGL Info:");
		VL_CORE_INFO("  {:<10}- {}", "Vendor", (char*)glGetString(GL_VENDOR));
		VL_CORE_INFO("  {:<10}- {}", "Renderer", (char*)glGetString(GL_RENDERER));
		VL_CORE_INFO("  {:<10}- {}", "Version", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}