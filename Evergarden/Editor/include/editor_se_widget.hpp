/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_se_widget.hpp
 * @brief   Shader editore widget.
 */

#ifndef H_EDITOR_SE_WIDGET
#define H_EDITOR_SE_WIDGET

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include <imgui/TextEditor.h>

namespace evergarden {

    class App;

    class Widget_ShaderEditor {
    public:
        Widget_ShaderEditor() = default;
        ~Widget_ShaderEditor() = default;
        void init();
        void onGuiRender();

    private:
        TextEditor                  m_editor;
        TextEditor::Coordinates     m_cursor_pos;
    };

}

#endif //! H_EDITOE_SE_WIDGET