/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_camera.hpp
 * @brief   A API package for camera class.
 */

#ifndef H_RENDER_CAMERA
#define H_RENDER_CAMERA

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace evergarden {

    enum class CameraType {
        None            = 0,
        Perspective     = 1,
        Orthographic    = 2
    };

    class Camera {
    public:
        Camera(float fov, float aspect, CameraType type = CameraType::Perspective, float nearPlane = 0.1f, float farPlane = 100.0f);
        const CameraType& _CameraType()             const { return m_camera_type; }

        const glm::vec3& _Position()                const { return m_position; }
        const glm::vec3& _UpVector()                const { return m_up; }
        const glm::vec3& _FrontVector()             const { return m_front; }
        const glm::vec3& _RightVector()             const { return m_right; }

        const float& _Pitch()                       const { return m_pitch; }
        const float& _Yaw()                         const { return m_yaw; }
        const float& _fov()                         const { return m_fov; }
        const float& _Aspect()                      const { return m_aspect; }

        const glm::mat4& _ProjectionMatrix()        const { return m_projectionMatrix; }
        const glm::mat4& _ViewMatrix()              const { return m_viewMatrix; }
        const glm::mat4& _ViewProjectionMatrix()    const { return m_viewProjectionMatrix; }

        const float& _NearPlane()                   const { return m_near; }
        const float& _FarPlane()                    const { return m_far; }

        void setPosition(const glm::vec3& position) {
            if (m_position == position)return;
            m_position = position; 
            UpdateViewMatrix();
        }

        void setPitch(const float pitch) {
            if (m_pitch == pitch)return;
            m_pitch = pitch; 
            UpdateViewMatrix();
        }

        void setYaw(const float yaw) {
            if (m_yaw == yaw)return;
            m_yaw = yaw; 
            UpdateViewMatrix();
        }

        void setfov(float fov) {
            if (m_fov == fov)return;
            m_fov = fov; 
            UpdateProjectionMatrix();
        }

        void setAspect(float aspect) {
            if (m_aspect == aspect)return;
            m_aspect = aspect; 
            UpdateProjectionMatrix();
        }

        void setCameraType(const CameraType& type) {
            if (m_camera_type == type)return;
            m_camera_type = type;
            m_pitch = 0.0f;
            m_yaw = -90.0f;
            UpdateProjectionMatrix();
            UpdateViewMatrix();
        }

        void setNearPlane(const float& val) {
            if (m_near == val)return;
            m_near = val;
            UpdateProjectionMatrix();
            UpdateViewMatrix();
        }

        void setFarPlane(const float& val) {
            if (m_far == val)return;
            m_far = val;
            UpdateProjectionMatrix();
            UpdateViewMatrix();
        }

    private:
        float       m_fov, m_aspect, m_far, m_near;
        float       m_pitch= -30.0f, m_yaw = -60.0f;
        CameraType  m_camera_type;

        glm::vec3   m_position = { -17.0f, 23.0f, 32.0f };
        glm::mat4   m_viewMatrix{};
        glm::mat4   m_projectionMatrix{};
        glm::mat4   m_viewProjectionMatrix{};
        glm::vec3   m_front = { 0.0f, 0.0f, -1.0f }, m_up = { 0.0f, 1.0f, 0.0f }, m_right = { 1.0f, 0.0f, 0.0f };
    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();
        void UpdateViewProjectionMatrix() { m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; }
    };

} //! namespace evergarden 

#endif //! H_RENDER_CAMERA
