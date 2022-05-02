/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_scene_layer.hpp
 * @brief   scene Layer.
 */

#ifndef H_EDITOR_SCENE_LAYER
#define H_EDITOR_SCENE_LAYER

#ifdef  WIN32
#pragma once
#endif //! WIN32


#include "editor_base.hpp"

#include "Renderer/include/render_camera.hpp"
#include "Renderer/include/render_scene.hpp"

#include "App/include/Application.hpp"

#include "Core/include/core_time.hpp"
#include "Core/include/core_event_sys.hpp"

namespace evergarden {

    class CameraController {
    public:
        CameraController(float fov, float aspect, CameraType type = CameraType::Perspective, float nearPlane = 0.1f, float farPlane = 100.0f, bool is_2D = false);
        void onUpdate(TimeFrame ts);
        void onEvent(Event& e);
        void onResize(float width, float height);

        Camera& GetCamera() { return c_Camera; }
        const Camera& GetCamera() const { return c_Camera; }

        void SetActive(const bool enable) { c_Enable = enable; }
        void SetSensitivity(const float sensitivity) { c_Sensitivity = sensitivity; }
        void SetSpeed(const float speed) { c_Speed = speed; }

        float& GetSensitivity() { return c_Sensitivity; }
        float& GetSpeed() { return c_Speed; }

    private:
        bool IsCameraControllerActive();
        bool IsMouseMiddlePress();

        bool __on_MouseScrolled__(MouseScrolledEvent& e);
        bool __on_MouseMoved__(MouseMovedEvent& e);
        bool __on_WindowResized__(WindowResizeEvent& e);

    private:
        Camera c_Camera;
        float c_Speed = 10.0f;
        float c_Last_X = 0, c_Last_Y = 0;
        float c_Sensitivity = 0.1f;
        float c_Enable = false;

    };

}

namespace evergarden {

    class Layer_MainCamera: public Layer {
    public:
        Layer_MainCamera();
        ~Layer_MainCamera() override = default;

        void onAttach() override {};
        void onDetach() override {};

        void onUpdate(TimeFrame timestep) override {};
        void onGuiRender() override;
        void onEvent(Event& event) override {};

        bool& GetWindowHandle() { return c_WindowHandle; }

    public:
        static Layer_MainCamera* GetInstance() { return s_Instance; };
        CameraController& GetCameraController() { return c_CameraController; }

    private:
        CameraController c_CameraController;
        static Layer_MainCamera* s_Instance;
        bool c_WindowHandle = true;
    };

}

namespace evergarden {

    struct RenderIPData {
    public:
        RenderIPData() {
            c_whiteTexture = Texture2D::create(1, 1);
            uint32_t defaultTextureData = 0xffffffff;
            c_whiteTexture->setData(&defaultTextureData, sizeof(uint32_t));
            c_checkboardTexture = Texture2D::create(__get_abs_path__("/Asset/image/ve_bkg.png")); //TODO
            c_color = glm::vec4{ 1.0f };
            c_transform = glm::mat4{ 1.0f };
        }

    public:
        std::shared_ptr<Texture2D> c_whiteTexture;
        std::shared_ptr<Texture2D> c_checkboardTexture;
        glm::vec4 c_color{};
        glm::mat4 c_transform{};
    };

    class Layer_Scene: public Layer {
    public:
        Layer_Scene(Layer_MainCamera *rhs);
        ~Layer_Scene() override;

    public:
        void onAttach() override;
        void onDetach() override;

        void onUpdate(TimeFrame timestep) override;
        void onGuiRender() override;
        void onEvent(Event& event) override;

        void BeginScene();
        void EndScene();
        void DrawGrid();

        static Layer_Scene* GetInstance() { return s_Instance; };
        std::shared_ptr<Scene>& GetScene() { return c_MainScene; }
        std::shared_ptr<SkyBox>& GetSkybox() { return c_SkyMap; }
        static std::shared_ptr<RenderIPData>& GetDefaultRenderData() { return c_DefaultRenderIPData; }

        bool IsShowGrid() const { return c_EnableGrid; }
        void SetShowGrid(const bool& enable) { c_EnableGrid = enable; }
        static void SetGuizmoFunc(std::function<void(glm::vec2)> func) { c_GuizmoFunc = std::move(func); }

    public:
        static Layer_Scene* s_Instance;
        std::shared_ptr<Scene> c_MainScene;
        Layer_MainCamera *MainCam_layer_instacne;

    private:
        std::shared_ptr<SkyBox> c_SkyMap;
        std::shared_ptr<FrameBuffer> c_FrameBuffer;
        std::shared_ptr<Shader> c_WireFrameShader;
        glm::vec2 c_ViewportSize = { 0.0f, 0.0f };
        std::shared_ptr<Mesh> c_Grid;
        bool c_EnableGrid = true;
        static std::shared_ptr<RenderIPData> c_DefaultRenderIPData;

        static std::function<void(glm::vec2)> c_GuizmoFunc;
    };

} //! namespace evergarden


#endif //! H_EDITOR_SCENE_LAYER