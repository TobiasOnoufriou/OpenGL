#pragma once

#include <glm/glm.hpp>

namespace GLCore::Utils {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float FoV, float aspect, float min_unit, float max_unit);

		void SetProjection(float FoV, float aspect, float min_unit, float max_unit);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

    const glm::vec3& GetDirection() const { return m_Direction; }
    void SetDirection(const glm::vec3& direction) { m_Direction = direction; RecalculateViewMatrix(); }

    const glm::vec3& GetUp() const { return m_Up; }
    void SetUp(const glm::vec3& up) { m_Up = up; RecalculateViewMatrix(); }

    const glm::vec3& GetRight() const { return m_Right; }
    void SetRight(const glm::vec3& right) { m_Right = right; RecalculateViewMatrix(); }


		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_Direction = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_Up = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_Right = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}
