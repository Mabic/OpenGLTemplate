#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm\glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 && position = glm::vec3(0.0f,0.0f,0.0f), 
           glm::vec3 && front = glm::vec3(0.0f, 0.0f, -1.0f), 
           glm::vec3 && up = glm::vec3(0.0f, 1.0f, 0.0f));
    ~Camera();

    glm::mat4 GetViewMatrix() const;
    void UpdateEulerAngles(float offsetX, float offsetY);

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;

    float m_pitch;
    float m_yaw;

    float m_sensitivity;
};

#endif // !CAMERA_HPP
