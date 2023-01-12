#include "glpch.h"
#include "PerspectiveCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"

namespace GLCore::Utils {

	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, bool rotation)
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
      m_Camera.SetPosition(-(m_Camera.GetDirection() * m_CameraTranslationSpeed * float(ts)));
		}

    if (Input::IsKeyPressed(HZ_KEY_LEFT_ALT)) {

      horizontal_angle_ += mouse_speed_ * float(1280 / 2 - Input::GetMouseX());
      vertical_angle_ += mouse_speed_ * float(720 / 2 - Input::GetMouseY());

      m_Camera.MouseChangeLookat(0.01f, horizontal_angle_, vertical_angle_);
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

  glm::vec3 PerspectiveCameraController::GetRaycastDirection(int mouse_x, int mouse_y) {
    float x = (float)(mouse_x) / (float)(WINDOW_WIDTH - 1);
    float y = 1.0 - (float)(mouse_y) / (float)(WINDOW_HEIGHT - 1);

    // Viewing vector
    glm::vec3 E = m_CameraPosition;
    glm::vec3 U = GetCamera().GetUp();
    glm::vec3 C = glm::normalize(GetCamera().GetLookAt() - m_CameraPosition); // implies viewing plane distancei s 1

    float phi = glm::radians(m_Camera.GetFoV() / 2.0);

    // Vector A = C x U
    glm::vec3 A = glm::normalize(glm::cross(C, U));
    // The REAL up vector B = A x C
    glm::vec3 B = glm::normalize(glm::cross(A, C));
    // View Center M = E + C
    glm::vec3 M = E + C;

    // V || B, but on NCD
    glm::vec3 V = B * glm::tan(phi);
    // H || A, but on NDC
    // If you didn't use theta here, you can simply use the ratio between this->width() and this->height()
    glm::vec3 H = A * glm::tan(phi) / (float)WINDOW_HEIGHT * (float)WINDOW_WIDTH;

    // Clicking point on the screen. World Coordinate.
    glm::vec3 P = M + float(2.0 * x - 1.0) * H + float(2.0 * y - 1.0) * V;

    /*
      m_cached_projection_plane_center = M;
      m_cached_projection_plane_xdir = H;
      m_cached_projection_plane_ydir = V;
    */

    glm::vec3 dir = glm::normalize(P - E);

    return dir;
  }

}