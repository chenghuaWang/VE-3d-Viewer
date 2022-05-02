/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/src/editor_main_layer.cpp
 * @brief   Main Layer contain multi layers.
 */

#include "Renderer/include/render_entity.hpp"
#include "Renderer/include/render_component.hpp"
#include "../include/editor_file_widget.hpp"
#include "../include/editor_main_layer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace evergarden {

    Entity      MainEditorWidget::c_selectEntity = Entity();
    std::string MainEditorWidget::c_flag_popup = "";
    bool        MainEditorWidget::c_flag_showHierarchy = true;
    bool        MainEditorWidget::c_flag_showInspector = true;
    bool        MainEditorWidget::c_flag_demoWindow = false;
    bool        MainEditorWidget::c_flag_styleEditor = false;
    bool        MainEditorWidget::c_flag_terminal = true;
    bool        MainEditorWidget::c_flag_init_fileDialog = false;
    bool        MainEditorWidget::c_flag_help_window = false;
    ImGuizmo::OPERATION MainEditorWidget::c_current_GizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE      MainEditorWidget::c_current_GizmoMode = ImGuizmo::WORLD;

    void MainEditorWidget::__get_file_dialog__(const std::string &label, const std::string &format, std::function<void(const std::string &)> func) {
        // A self defined file get functions. Used file_widget.
        // You need to enter in a label for window show. And a format class for get format to choose.
        GuiFileWidget::_on_open_window_(label, format);
        GuiFileWidget::_on_window_react_(func);
    }

    void MainEditorWidget::AddObjectMenu(Entity& entity) {
        if (ImGui::BeginMenu("New Object"))
        {
            for (auto item : MeshType2Item)
                if (ImGui::MenuItem(item.second.c_str()))
                {
                    c_flag_popup = item.second;
                    SetSelectEntity(entity);
                }
            ImGui::EndMenu();
        }
    }

    void MainEditorWidget::AddObjectAnswer() {
        if (c_flag_popup != MeshType2Item[MeshType::Model] && MeshItem2Type.count(c_flag_popup) > 0)
        {
            ImGui::Begin(c_flag_popup.c_str());

            ImGui::Text("Transform");
            static glm::vec3 position{ 0.0f };
            static glm::vec3 rotation{ 0.0f };
            static glm::vec3 scale{ 1.0f };
            ImGui::DragFloat3("Position", (float*)&position, 0.1f);
            ImGui::DragFloat3("Rotation", (float*)&rotation, 1.0f);
            ImGui::DragFloat3("Scale", (float*)&scale, 0.01f);

            ImGui::Separator();
            ImGui::Text("Name");
            static char buf[32] = "New Object";
            ImGui::InputText("##edit", buf, 50);

            ImGui::Separator();

            if (ImGui::Button("Create"))
            {
                auto entity = c_selectEntity.CreateChild(std::string(buf));
                if (MeshItem2Type[c_flag_popup] != MeshType::None)
                    entity.AddComponent<MeshComponent>(MeshItem2Type[c_flag_popup]);
                entity.GetComponent<TransformComponent>().Position = position;
                entity.GetComponent<TransformComponent>().Rotation = rotation;
                entity.GetComponent<TransformComponent>().Scale = scale;
                entity.GetComponent<TransformComponent>().Update();
                c_flag_popup = "";
                c_selectEntity = entity;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                c_flag_popup = "";
            }
            ImGui::End();
        } else if (c_flag_popup == MeshType2Item[MeshType::Model] && MeshItem2Type.count(c_flag_popup) > 0){
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Model", ".obj", ".");
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string path = ImGuiFileDialog::Instance()->GetCurrentPath();
                std::string name = ImGuiFileDialog::Instance()->GetCurrentFileName();
                path = path + "/" + name;
                std::cout << "INFO: Get selected path " << path << std::endl;
                auto entity = c_selectEntity.CreateChild(std::string("mesh"));
                entity.AddComponent<MeshComponent>(path);
                c_flag_popup = "";
                c_selectEntity = entity;
            } else {
                c_flag_popup = "";
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
        
    }

    void MainEditorWidget::SetSelectEntity_IP(Scene *rhs) {
        c_selectEntity.c_Scene = rhs;
    }

} //! namespace evergarden

namespace evergarden {

    void _widget_4_main_layer_::__show_MainMenu__(Layer_Scene *s_ip, MainEditorWidget * s_ew, Layer_MainCamera *s_mc) {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Edit"))
            {
                bool linemode = s_ip->GetScene()->GetLineMode();
                std::string display = "";
                if ( linemode ) display = "polygon";
                else display = "wireframe";
                if (ImGui::MenuItem(display.c_str())) linemode = !linemode;
                s_ip->GetScene()->SetLineMode(linemode);
                bool gridFlag = s_ip->GetInstance()->IsShowGrid();
                ImGui::MenuItem("Grid", NULL, &gridFlag);
                s_ip->SetShowGrid(gridFlag);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Environment"))
            {
                //Load Skybox
                if (ImGui::BeginMenu("Skybox"))
                {
                    ImGui::Image((void*)s_ip->GetSkybox()->getTextureID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
                    if (ImGui::BeginPopupContextItem("Delete Skybox"))
                    {
                        if (ImGui::MenuItem("Delete"))
                        {
                            s_ip->GetSkybox().reset();
                            s_ip->GetSkybox() = SkyBox::create();
                        }
                        ImGui::EndPopup();
                    }
                    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                        s_ew->SetPopupFlag("Choose Skybox");

                    if (ImGui::MenuItem("Create")) 
                    {
                        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey_skbox", "HDR Skybox", ".hdr", ".");
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            // TODO file chooser
            // s_ew->__get_file_dialog__("Choose Skybox", ".hdr", [&](const std::string& filePathName) {
            //     s_ip->GetSkybox().reset();
            //     s_ip->GetSkybox() = SkyBox::create(filePathName);
            // });

            if (ImGui::BeginMenu("Obejct"))
            {
                MeshType type = MeshType::None;
                s_ew->AddObjectMenu(s_ew->GetSelectEntity());
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Component"))
            {
                if (ImGui::MenuItem("Material Component"))
                    s_ew->AddComponent<MaterialComponent>();
                if (ImGui::MenuItem("Mesh Component"))
                    s_ew->AddComponent<MeshComponent>();
                if (ImGui::MenuItem("Light Component"))
                    s_ew->AddComponent<LightComponent>();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Hierachy", NULL, &s_ew->c_flag_showHierarchy);
                ImGui::MenuItem("Inspector", NULL, &s_ew->c_flag_showInspector);
                ImGui::MenuItem("StyleEditor", NULL, &s_ew->c_flag_styleEditor);
                ImGui::MenuItem("Terminal", NULL, &s_ew->c_flag_terminal);
                ImGui::MenuItem("Show Demo Window", NULL, &s_ew->c_flag_demoWindow);
                ImGui::MenuItem("Camera Setting", NULL, &s_mc->GetWindowHandle());
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("About"))
            {
                if (ImGui::MenuItem("Help")) 
                {

                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // TODO For display file choose 
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey_skbox"))
                {
                    // action if OK
                    if (ImGuiFileDialog::Instance()->IsOk()) {
                        std::string path = ImGuiFileDialog::Instance()->GetCurrentPath();
                        std::string name = ImGuiFileDialog::Instance()->GetCurrentFileName();
                        path = path + "/" + name;
                        s_ip->GetSkybox().reset();
                        s_ip->GetSkybox() = SkyBox::create(path);
                    } else {
                        ImGuiFileDialog::Instance()->Close();
                    }
                    // close
                    ImGuiFileDialog::Instance()->Close();
                }
    };

    void _widget_4_main_layer_::__show_GuiZmoMenu__(MainEditorWidget* E_rhs) {
        if (MainEditorWidget::c_flag_showInspector)
        {
            ImGui::Begin("Inspector", &MainEditorWidget::c_flag_showInspector);
            if (ImGui::RadioButton("Translate", MainEditorWidget::c_current_GizmoOperation == ImGuizmo::TRANSLATE))
            {
                MainEditorWidget::c_current_GizmoOperation = ImGuizmo::TRANSLATE;
                MainEditorWidget::c_current_GizmoMode = ImGuizmo::WORLD;
            }

            if (ImGui::RadioButton("Rotate", MainEditorWidget::c_current_GizmoOperation == ImGuizmo::ROTATE))
            {
                MainEditorWidget::c_current_GizmoOperation = ImGuizmo::ROTATE;
                MainEditorWidget::c_current_GizmoMode = ImGuizmo::LOCAL;
            }

            if (ImGui::RadioButton("Scale", MainEditorWidget::c_current_GizmoOperation == ImGuizmo::SCALE))
            {
                MainEditorWidget::c_current_GizmoOperation = ImGuizmo::SCALE;
            }
            ImGui::End();
        }
    };

    void _widget_4_main_layer_::__show_Help__() {
        if (ImGui::Begin("Help")) 
        {
        ImGui::Text("Violet Evergarden.\n Under MIT Licence.\n Made with love abd passion.\n");

        ImGui::End();
        }
    }

} //! namespace evergarden

namespace evergarden {

    Layer_Main::Layer_Main(): Layer("Main Layer") {
    };

    Layer_Main::~Layer_Main() {
        ///< Don't need to delete layer of camera and scene. Layer stack will done this by cpp RAII 
        // delete MainEditor_Widget_instance;
    }

    void Layer_Main::onAttach() {
        Layer_Scene_instance->SetGuizmoFunc([&](glm::vec2 viewportSize) {
            ImVec2 size = ImGui::GetContentRegionAvail();
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();
            ImGuizmo::SetRect(cursorPos.x, cursorPos.y, size.x, size.y);
            if (MainEditor_Widget_instance->GetSelectEntity()&&!MainEditor_Widget_instance->GetSelectEntity().IsRoot())
            {
                ImGuizmo::Manipulate(glm::value_ptr(Layer_MainCamera_instance->GetCameraController().GetCamera()._ViewMatrix()),
                                     glm::value_ptr(Layer_MainCamera_instance->GetCameraController().GetCamera()._ProjectionMatrix()),
                                     MainEditor_Widget_instance->c_current_GizmoOperation, MainEditor_Widget_instance->c_current_GizmoMode,
                                     &MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Transform[0][0]
                );
                ImGuizmo::DecomposeMatrixToComponents(&MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Transform[0][0],
                                                      &MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Position[0],
                                                      &MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Rotation[0],
                                                      &MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Scale[0]);
                MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Position = MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().Position - MainEditor_Widget_instance->GetSelectEntity().GetComponent<TransformComponent>().RelatePosition;
            }
        });
    }

    void Layer_Main::init() {
        MainEditorWidget::SetSelectEntityScene(Layer_Scene_instance);
    }

    void Layer_Main::onUpdate() {
        if (Layer_MainCamera_instance == nullptr || Layer_Scene_instance == nullptr) {
            std::cout << "Err: instance not init with true memory." <<std::endl;
        }
    }

    void Layer_Main::onGuiRender() {
        _widget_4_main_layer_::__show_MainMenu__(Layer_Scene_instance, MainEditor_Widget_instance, Layer_MainCamera_instance);
        _widget_4_main_layer_::__show_GuiZmoMenu__(MainEditor_Widget_instance);
        
        if (MainEditorWidget::c_flag_demoWindow) {
            ImGui::ShowDemoWindow();
        }
        MainEditorWidget::AddObjectAnswer();
    }

} //! namespace evergarden

