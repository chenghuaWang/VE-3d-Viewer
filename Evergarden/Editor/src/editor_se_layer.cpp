#include "../include/editor_se_layer.hpp"

namespace evergarden {

    Layer_ShaderEditor::Layer_ShaderEditor(): Layer("Shader Editor") {
        m_se.init();
    };

    void Layer_ShaderEditor::onGuiRender() {

        if (m_window_on){
            ImGui::Begin("Shader Editor", &m_window_on, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
            m_se.onGuiRender();
            ImGui::End();
        }
        
    }

}