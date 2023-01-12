#include "glpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLCore::Utils {

	PerspecitiveCamera::PerspecitiveCamera(float FoV, float aspect, float min_unit, float max_unit)
		: m_ProjectionMatrix(glm::perspective(FoV, aspect, min_unit, max_unit)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspecitiveCamera::SetProjection(float FoV, float aspect, float min_unit, float max_unit)
	{
    m_ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect, min_unit, max_unit);
  
		//m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

  void PerspecitiveCamera::MouseChangeLookat(float coe, float dx, float dy) {
    glm::vec3 vdir(GetLookAt() - GetPosition());
    glm::vec3 u(glm::normalize(glm::cross(vdir, GetUp())));
    glm::vec3 v(glm::normalize(glm::cross(u, vdir)));

    m_LookAt += coe * (dy * v - dx * u);
    RecalculateViewMatrix();
  }

	void PerspecitiveCamera::RecalculateViewMatrix()
	{
    float r_head = glm::radians(m_Head), r_pitch = glm::radians(m_Pitch);
    m_Position.x = m_LookAt.x + m_Eye_Distance * glm::cos(r_head) * glm::cos(r_pitch);
    m_Position.y = m_LookAt.y + m_Eye_Distance * glm::sin(r_head);
    m_Position.z = m_LookAt.z + m_Eye_Distance * glm::cos(r_head) * glm::sin(r_pitch);

    m_Up = glm::vec3(0.0f, (glm::cos(r_head) > 0.0) ? 1.0f : -1.0, 0.0);
    m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, m_Up);

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}