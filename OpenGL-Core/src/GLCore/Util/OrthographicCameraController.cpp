#include "glpch.h"
#include "OrthographicCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"

namespace GLCore::Utils {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(45, m_AspectRatio, 0.1f, 100.0f), m_Rotation(rotation)
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

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{

 

		if (Input::IsKeyPressed(HZ_KEY_A))
		{
      m_CameraPosition -= m_Camera.GetRight() * m_CameraTranslationSpeed * float(ts);

			//m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			//m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
      m_CameraPosition += m_Camera.GetRight() * m_CameraTranslationSpeed * float(ts);

			//m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			//m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(HZ_KEY_W))
		{
      m_CameraPosition += m_Camera.GetDirection() * m_CameraTranslationSpeed * float(ts);

			//m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			//m_CameraPosition.z += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
      m_CameraPosition -= m_Camera.GetDirection() * m_CameraTranslationSpeed * float(ts);

      //m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			//m_CameraPosition.z -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

    if (Input::IsKeyPressed(HZ_KEY_LEFT_ALT)) {
      horizontal_angle_ += mouse_speed_ * float(800 / 2 - Input::GetMouseX());
      vertical_angle_ += mouse_speed_ * float(600 / 2 - Input::GetMouseY());
      
      m_Camera.SetDirection(glm::vec3 {
        cos(vertical_angle_) * sin(horizontal_angle_),
        sin(vertical_angle_),
        cos(vertical_angle_) * cos(horizontal_angle_)}
      );

      m_Camera.SetRight(glm::vec3 {
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

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(90, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(90, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}