/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_main_layer.hpp
 * @brief   Main Layer contain multi layers.
 */

#ifndef H_EDITOR_MAIN_LAYER
#define H_EDITOR_MAIN_LAYER

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "editor_base.hpp"
#include "editor_layer.hpp"
#include "editor_scene_layer.hpp"
#include "Renderer/include/render_scene.hpp"

namespace evergarden {

    static std::unordered_map<MeshType, std::string> MeshType2Item = {
        { MeshType::None, "Empty" },
        { MeshType::Plane, "Plane" },
        { MeshType::Cube, "Cube" },
        { MeshType::Sphere, "Sphere" },
        { MeshType::Model, "Model" },
    };
    static std::unordered_map<std::string, MeshType> MeshItem2Type = {
        { "Empty", MeshType::None },
        { "Plane", MeshType::Plane },
        { "Cube", MeshType::Cube },
        { "Sphere", MeshType::Sphere },
        { "Model", MeshType::Model }
    };

    class MainEditorWidget{
    public:
        static void     SetSelectEntity(Entity& entity) { c_selectEntity = entity; }
        static void     SetSelectEntityScene(Layer_Scene *rhs) { c_selectEntity.c_Scene = rhs->GetScene().get(); };
        static void     SetPopupFlag(const std::string& flag) { c_flag_popup = flag; }
        static Entity&  GetSelectEntity() { return c_selectEntity; }

        static void __get_file_dialog__(const std::string& label, const std::string& format, std::function<void(const std::string&)> func);

        static void ShowTexture(const char* label, Scene *Render_instance, const std::shared_ptr<Texture2D>& texture, std::function<void(void)> func = []() {}) {}; //TODO

        static void Rename(Entity& entity) {}; //TODO

        static void AddObjectMenu(Entity& entity);
        static void AddObjectAnswer();

        static void SetSelectEntity_IP(Scene *rhs); 

        template<typename T>
        static void SetDragDropSource(const char* label, std::function<void(void)> func, T& data) {
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload(label, &data, sizeof(T));
                func();
                ImGui::EndDragDropSource();
            }
        }

        template<typename T>
        static void SetDragDropTarget(const char* label, std::function<void(T&)> func) {
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(label)) {
                    T data = *(const T*)payload->Data;
                    func(data);
                }
                ImGui::EndDragDropTarget();
            }
        }

        template<typename T>
        static void AddComponent() {
            if (!MainEditorWidget::GetSelectEntity().HasComponent<T>() && !MainEditorWidget::GetSelectEntity().IsRoot())
                MainEditorWidget::GetSelectEntity().AddComponent<T>();
        }
    
    public:
        static bool c_flag_showHierarchy;
        static bool c_flag_showInspector;
        static bool	c_flag_demoWindow;
        static bool c_flag_styleEditor;
        static bool c_flag_terminal;
        static bool c_flag_help_window;

        static      ImGuizmo::OPERATION c_current_GizmoOperation;
        static      ImGuizmo::MODE      c_current_GizmoMode;

        static bool c_flag_init_fileDialog;

    private:
        static Entity       c_selectEntity;
        static std::string  c_flag_popup;

    };

} //! namespace evergarden

namespace evergarden {

    /*!
     * @brief Class for main layer to use. Multi Layer window
     * @details This class can only be used in ImGui context.
     */
    class _widget_4_main_layer_ {
    public:
        static void __show_MainMenu__(Layer_Scene *s_ip, MainEditorWidget * s_ew, Layer_MainCamera *s_mc);
        static void __show_GuiZmoMenu__(MainEditorWidget* E_rhs);
        static void __show_Help__();

    };

} //! namspace evergarden


namespace evergarden {

    class Layer_Main: public Layer {
    public:
        Layer_Main();
        ~Layer_Main();
    
    public:
        void onAttach()         override;
        void onDetach()         override {};
        void onUpdate()         override;
        void onGuiRender()      override;
        void onEvent(Event& e)  override {};
        void init();

    public:
        void setScene_instacne(Layer_Scene *rhs) { Layer_Scene_instance = rhs; };
        void setMainCamera_instance(Layer_MainCamera *rhs) { Layer_MainCamera_instance = rhs; }

    private:
        Layer_Scene             *Layer_Scene_instance = nullptr;
        Layer_MainCamera        *Layer_MainCamera_instance = nullptr;
        MainEditorWidget        *MainEditor_Widget_instance = nullptr;

    };

} //! namespace evergarden

#endif //! H_EDITOR_MAIN_LAYER