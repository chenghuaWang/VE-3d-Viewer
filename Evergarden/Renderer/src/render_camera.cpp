/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_camera.hpp
 * @brief   A API package for camera class.
 */

#include "../include/render_camera.hpp"

namespace evergarden {

    Camera::Camera(float fov, float aspect, CameraType type, float nearPlane, float farPlane):
        m_fov(fov), m_aspect(aspect), m_camera_type(type), m_near(nearPlane), m_far(farPlane){
        UpdateProjectionMatrix();
        UpdateViewMatrix();
    }

    void Camera::UpdateViewMatrix() {
        auto front = glm::vec3(1.0f);

        front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
        front.y = sin(glm::radians(m_pitch));
        front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

        m_up = glm::normalize(glm::cross(m_right, m_front));

        m_viewMatrix = glm::lookAt(m_position, m_front + m_position, m_up);
        UpdateViewProjectionMatrix();
    }

    void Camera::UpdateProjectionMatrix() {
        switch (m_camera_type) {
            case CameraType::None:
                break;
            case CameraType::Orthographic:
                m_projectionMatrix = glm::ortho(-glm::radians(m_fov), glm::radians(m_fov), -glm::radians(m_fov) / m_aspect, glm::radians(m_fov) / m_aspect, m_near, m_far );
                UpdateViewProjectionMatrix();
                break;
            case CameraType::Perspective:
                m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
                UpdateViewProjectionMatrix();
                break;
        }
    }

}
