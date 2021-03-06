#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 && position, glm::vec3 && front, glm::vec3 && up)
    : m_position(std::move(position)),
      m_front(std::move(front)),
      m_up(std::move(up)),
      m_yaw(-90.0f),
      m_pitch(0.0f),
      m_sensitivity(0.1f),
	  m_speed(0.001f)
{}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::UpdateEulerAngles(float offsetX, float offsetY)
{	
    m_yaw += (offsetX * m_sensitivity);
    m_pitch += (offsetY * m_sensitivity);
    
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
}

void Camera::UpdatePosition(Camera::DIRECTION direction) {
	switch (direction) {
	case DIRECTION::FORWARD :
		m_position += m_front * m_speed;
		break;
	case DIRECTION::BACKWARD :
		m_position -= m_front * m_speed;
		break;
	case DIRECTION::RIGHT :
		m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;
		break;
	case DIRECTION::LEFT :
		m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
		break;
	default:
		assert("Undefined direction!");
	}
}