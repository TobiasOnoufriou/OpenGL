#include "glpch.h"
#include "PerspectiveCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"

namespace GLCore::Utils {

	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(45, m_AspectRatio, -1.0f, 1.0f), m_Rotation(rotation)
	{
    m_Camera.SetDirection(glm::vec3{
      cos(vertical_angle_) * sin(horizontal_angle_),
      sin(vertical_angle_),
      cos(vertical_angle_) * cos(horizontal_angle_) }
    );

    m_Camera.SetRight(glm::vec3{
     sin(horizontal_angle_ - 3.14f / 2.0f),
     0,
     cos(horizontal_angle_ - 3.14f / 2.0f)
    });

    m_Camera.SetUp(glm::cross(m_Camera.GetRight(), m_Camera.GetDirection()));
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_A))
		{
      m_CameraPosition -= m_Camera.GetRight() * m_CameraTranslationSpeed * float(ts);
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
      m_CameraPosition += m_Camera.GetRight() * m_CameraTranslationSpeed * float(ts);
		}

		if (Input::IsKeyPressed(HZ_KEY_W))
		{
      m_CameraPosition += m_Camera.GetDirection() * m_CameraTranslationSpeed * float(ts);
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
      m_CameraPosition -= m_Camera.GetDirection() * m_CameraTranslationSpeed * float(ts);
		}

    if (Input::IsKeyPressed(HZ_KEY_LEFT_ALT)) {
      horizontal_angle_ += mouse_speed_ * float(1280 / 2 - Input::GetMouseX());
      vertical_angle_ += mouse_speed_ * float(720 / 2 - Input::GetMouseY());

      m_Camera.SetDirection(glm::vec3{
        cos(vertical_angle_) * sin(horizontal_angle_),
        sin(vertical_angle_),
        cos(vertical_angle_) * cos(horizontal_angle_) }
      );

      m_Camera.SetRight(glm::vec3{
        sin(horizontal_angle_ - 3.14f / 2.0f),
        0,
        cos(horizontal_angle_ - 3.14f / 2.0f)
        });

      m_Camera.SetUp(glm::cross(m_Camera.GetRight(), m_Camera.GetDirection()));
    }

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f) 
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

    m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
    return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
  {
    return false;
	}

}