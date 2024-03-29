#pragma once

#include "OrthographicCamera.h"

#include "Velvet/Events/ApplicationEvent.h"
#include "Velvet/Events/MouseEvent.h"
#include "Velvet/Core/Timestep.h"

namespace Velvet {

	class OrthographicCameraController
	{
	public:
		struct CameraSettings
		{
			bool rotation = false;
			bool zoom = true;
		};
	public:
		OrthographicCameraController(bool rotation = false, bool zoom = true);
		OrthographicCameraController(glm::vec2& initialWindowDimensions, bool rotation = false, bool zoom = true);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetScale() const { return m_Scale; }
		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

		void SetCameraSettings(CameraSettings settings) { m_Rotation = settings.rotation; m_Zoom = settings.zoom; }
		void SetInitialWindowDimensions(const glm::vec2& initialWindowDimensions);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_Scale;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		bool m_Zoom;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; // In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 2.0f, m_CameraRotationSpeed = 120.0f;
	};

}