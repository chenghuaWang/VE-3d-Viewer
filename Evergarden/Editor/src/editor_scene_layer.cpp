#include "../include/editor_scene_layer.hpp"

namespace evergarden {

    Layer_Scene* Layer_Scene::s_Instance = nullptr;
    std::shared_ptr<RenderIPData> Layer_Scene::c_DefaultRenderIPData = nullptr;
    std::function<void(glm::vec2)> Layer_Scene::c_GuizmoFunc = [](glm::vec2) {};

    Layer_Scene::Layer_Scene(Layer_MainCamera *rhs): MainCam_layer_instacne(rhs), Layer("Scene Layer") {
        s_Instance = this;
        c_MainScene = std::make_shared<Scene>();
        c_DefaultRenderIPData = std::make_shared<RenderIPData>();
    }

    Layer_Scene::~Layer_Scene() {

    }

    void Layer_Scene::onAttach() {
        c_Grid = Mesh::Create(MeshType::Plane, 20);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        FrameBufferElement fbSpec;
        fbSpec.Width = 1920;
        fbSpec.Height = 1080;
        c_FrameBuffer = FrameBuffer::Create(fbSpec);

        c_WireFrameShader = Shader::create(__get_abs_path__("/Asset/shader/default.shader")); //TODO

        c_SkyMap = SkyBox::create();
    }

    void Layer_Scene::onDetach() {
        Layer::onDetach();
    }

    void Layer_Scene::onUpdate(TimeFrame timestep) {
        BeginScene();
        MainCam_layer_instacne->GetCameraController().onUpdate(timestep);
        c_MainScene->RenderObject(MainCam_layer_instacne->GetCameraController().GetCamera(), c_SkyMap);
        EndScene();
    }

    void Layer_Scene::onGuiRender() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        MainCam_layer_instacne->GetCameraController().SetActive(ImGui::IsWindowFocused());
        
        if (c_ViewportSize != *((glm::vec2*) & viewportPanelSize)) {
            c_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };
            c_FrameBuffer->Resize(c_ViewportSize.x, c_ViewportSize.y);
            MainCam_layer_instacne->GetCameraController().onResize(c_ViewportSize.x, c_ViewportSize.y);
        }
        
        c_GuizmoFunc(c_ViewportSize);
        uint32_t textureID = c_FrameBuffer->GetColorAttachmentID();
        ImGui::Image((void*)textureID, ImVec2(c_ViewportSize.x, c_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void Layer_Scene::onEvent(Event &event) {
        MainCam_layer_instacne->GetCameraController().onEvent(event);
    }

    void Layer_Scene::BeginScene() {
        c_FrameBuffer->Bind();
        glClearColor(0.25f, 0.25f, 0.25f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawGrid();
    }

    void Layer_Scene::EndScene() {
        if (!Layer_Scene::GetInstance()->GetScene()->GetLineMode()) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            c_SkyMap->bindSkyBox(MainCam_layer_instacne->GetCameraController().GetCamera());
        }
        c_FrameBuffer->Unbind();
    }

    void Layer_Scene::DrawGrid() {
        c_WireFrameShader->bind();
        c_WireFrameShader->setMat4("cpu_ViewProjection", MainCam_layer_instacne->GetCameraController().GetCamera()._ViewProjectionMatrix());
        c_WireFrameShader->setMat4("cpu_Transform", glm::scale(c_DefaultRenderIPData->c_transform, glm::vec3(20.0f)));
        c_DefaultRenderIPData->c_whiteTexture->bind();
        if (c_EnableGrid) {
            c_Grid->Draw(true);
        }
    }

    Layer_MainCamera* Layer_MainCamera::s_Instance = nullptr;

    Layer_MainCamera::Layer_MainCamera():
        Layer("WindowCamera"), c_CameraController(23.0f, 16.0f / 9.0f, CameraType::Perspective) {}

    void Layer_MainCamera::onGuiRender() {
        if (c_WindowHandle) {
            ImGui::Begin("Camera Setting", &c_WindowHandle);
            ImGui::Text("Camera");

            float fov = c_CameraController.GetCamera()._fov();
            ImGui::SliderFloat("fov", &fov, 0.1f, 45.0f, "%.1f");
            c_CameraController.GetCamera().setfov(fov);

            float sensitivity = c_CameraController.GetSensitivity();
            ImGui::SliderFloat("sensitivity", &sensitivity, 0.01f, 1.00f, "%.2f");
            c_CameraController.SetSensitivity(sensitivity);

            float speed = c_CameraController.GetSpeed();
            ImGui::SliderFloat("speed", &speed, 0.1f, 30.0f, "%.1f");
            c_CameraController.SetSpeed(speed);

            ImGui::Separator();

            glm::vec3 position = c_CameraController.GetCamera()._Position();
            ImGui::DragFloat3("Position", (float*)&position, 0.1f);
            c_CameraController.GetCamera().setPosition(position);

            ImGui::Separator();

            ImGui::Text("Rotation setting");
            float pitch = c_CameraController.GetCamera()._Pitch();
            ImGui::DragFloat("Pitch", &pitch, 1.0f);
            c_CameraController.GetCamera().setPitch(pitch);
            float yaw = c_CameraController.GetCamera()._Yaw();
            ImGui::DragFloat("Yaw", &yaw, 1.0f);
            c_CameraController.GetCamera().setYaw(yaw);

            ImGui::End();
        }
    }

} //! namespace evergarden

namespace evergarden {

    CameraController::CameraController(float fov, float aspect, CameraType type, float nearPlane, float farPlane,
                                       bool is_2D):c_Camera(fov, aspect, type, nearPlane, farPlane) {}

    void CameraController::onUpdate(TimeFrame ts) {
        glm::vec3 position = c_Camera._Position();
        auto window = App::getInstance()->getWindow()->getPlatformWindow();

        if (IsCameraControllerActive() || IsMouseMiddlePress()) {
            Input::SetCursorHidden(window, true);
        
            if (Input::IsKeyPressed(window, KeyCodes::A))
                position -= c_Speed * ts.getSeconds() * c_Camera._RightVector();
            if (Input::IsKeyPressed(window, KeyCodes::D))
                position += c_Speed * ts.getSeconds() * c_Camera._RightVector();
            if (Input::IsKeyPressed(window, KeyCodes::W))
                position += c_Speed * ts.getSeconds() * c_Camera._FrontVector();
            if (Input::IsKeyPressed(window, KeyCodes::S))
                position -= c_Speed * ts.getSeconds() * c_Camera._FrontVector();
            if (Input::IsKeyPressed(window, KeyCodes::Q))
                position += c_Speed * ts.getSeconds() * c_Camera._UpVector();
            if (Input::IsKeyPressed(window, KeyCodes::E))
                position -= c_Speed * ts.getSeconds() * c_Camera._UpVector();
        }
        else
            Input::SetCursorHidden(window,false);
        c_Camera.setPosition(position);
    }

    // std::bind(&fn,this,std::placeholders::_1)
    void CameraController::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&CameraController::__on_MouseScrolled__, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseMovedEvent>(std::bind(&CameraController::__on_MouseMoved__, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&CameraController::__on_WindowResized__, this, std::placeholders::_1));
    }

    void CameraController::onResize(float width, float height) {
        float aspect = width / height;
        c_Camera.setAspect(aspect);
    }

    bool CameraController::IsCameraControllerActive() {
        auto window = App::getInstance()->getWindow()->getPlatformWindow();
        return Input::IsMouseButtonPressed(window, MouseCode::ButtonRight) && c_Enable;
    }

    bool CameraController::IsMouseMiddlePress() {
        auto window = App::getInstance()->getWindow()->getPlatformWindow();
        return Input::IsMouseButtonPressed(window, MouseCode::ButtonMiddle) && c_Enable;
    }

    bool CameraController::__on_MouseScrolled__(MouseScrolledEvent &e) {
        if (IsCameraControllerActive()) {
            float c_Zoom = c_Camera._fov();
            c_Zoom -= e.GetYOffset();
            if (c_Camera._CameraType() == CameraType::Perspective) {
                if (c_Zoom <= 1.0f)
                    c_Zoom = 1.0f;
                if (c_Zoom >= 45.0f)
                    c_Zoom = 45.0f;
            }
            c_Camera.setfov(c_Zoom);
        }
        return false;
    }

    bool CameraController::__on_MouseMoved__(MouseMovedEvent &e) {
        auto window = App::getInstance()->getWindow()->getPlatformWindow();
        if (IsCameraControllerActive()) {
            if (c_Camera._CameraType() == CameraType::Perspective) {
                float yaw = c_Camera._Yaw(), pitch = c_Camera._Pitch();
                yaw += (e.GetX() - c_Last_X) * c_Sensitivity;
                pitch -= (e.GetY() - c_Last_Y) * c_Sensitivity;
                if (pitch > 89.0f)
                    pitch = 89.0f;
                else if (pitch < -89.0f)
                    pitch = -89.0f;
                c_Camera.setPitch(pitch);
                c_Camera.setYaw(yaw);
            }
            else if (c_Camera._CameraType() == CameraType::Orthographic) {
                glm::vec3 position = c_Camera._Position();
                position += (e.GetX() - c_Last_X) * c_Camera._RightVector() * c_Sensitivity * 10.0f;
                position -= (e.GetY() - c_Last_Y) * c_Camera._UpVector() * c_Sensitivity * 10.0f;
                c_Camera.setPosition(position);
            }
        }
        else if (IsMouseMiddlePress()) {
            glm::vec3 position = c_Camera._Position();
            position += (e.GetX() - c_Last_X) * c_Camera._RightVector() * c_Sensitivity / 100.0f;
            position -= (e.GetY() - c_Last_Y) * c_Camera._UpVector() * c_Sensitivity / 100.0f;
            c_Camera.setPosition(position);
        }
        c_Last_X = e.GetX();
        c_Last_Y = e.GetY();
        return false;
    }

    bool CameraController::__on_WindowResized__(WindowResizeEvent &e) {
        if (e.GetWidth() != 0 && e.GetHeight() != 0) {
            float aspect = (float)e.GetWidth() / (float)e.GetHeight();
            c_Camera.setAspect(aspect);
        }
        return false;
    }


}

