/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_file_widget.cpp
 * @brief   Main Layer contain multi layers.
 */

#ifndef H_EDITOR_FILE_WIDGET
#define H_EDITOR_FILE_WIDGET

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "editor_base.hpp"

namespace evergarden {

    class GuiFileWidget {
    public:
        static void _on_open_window_(const std::string &_describe_, const std::string &_class_);

        static void _on_window_react_(std::function<void(const std::string &)> func);

    private:
        // static std::string path_got;

    };

} //! namespace evergarden

#endif //! H_EDITOR_FILE_WIDGET