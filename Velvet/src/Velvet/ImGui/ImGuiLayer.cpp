#include <vlpch.h>
#include "Velvet/ImGui/ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Velvet/Core/Application.h"
#include "Velvet/Core/KeyCodes.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Velvet {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Velvet key codes
		io.KeyMap[ImGuiKey_Tab] = VL_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = VL_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VL_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VL_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VL_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VL_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = VL_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = VL_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = VL_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = VL_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = VL_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VL_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = VL_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = VL_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = VL_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = VL_KEY_A;
		io.KeyMap[ImGuiKey_C] = VL_KEY_C;
		io.KeyMap[ImGuiKey_V] = VL_KEY_V;
		io.KeyMap[ImGuiKey_X] = VL_KEY_X;
		io.KeyMap[ImGuiKey_Y] = VL_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = VL_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(VL_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[VL_KEY_LEFT_CONTROL] || io.KeysDown[VL_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[VL_KEY_LEFT_SHIFT] || io.KeysDown[VL_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[VL_KEY_LEFT_ALT] || io.KeysDown[VL_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[VL_KEY_LEFT_SUPER] || io.KeysDown[VL_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		
		// TEMPORARY
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

}