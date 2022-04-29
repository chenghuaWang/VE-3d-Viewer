/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_se_layer.hpp
 * @brief   Shader editore layer.
 */

#ifndef H_EDITOR_SE_LAYER
#define H_EDITOR_SE_LAYER

#ifdef  WIN32
#pragma once
#endif

#include "editor_layer.hpp"
#include "editor_se_widget.hpp"

namespace evergarden {

    class Layer_ShaderEditor: public Layer {
    public:
        Layer_ShaderEditor();
        ~Layer_ShaderEditor() override = default;

    public:
        void onAttach()         override {};
        void onDetach()         override {};
        void onUpdate()         override {};
        void onGuiRender()      override;
        void onEvent(Event& e)  override {};

    private:
        bool                    m_window_on = true;
        Widget_ShaderEditor     m_se {};
    };

}

#endif

