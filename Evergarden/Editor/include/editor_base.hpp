/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_base.hpp
 * @brief   The base file provide some basic define and macros
 */

#ifndef H_EDITOR_BASE_
#define H_EDITOR_BASE_

#ifdef WIN32
#pragma once
#endif //! WIN32

#include <memory>
#include <string>

#define EDITOR_PXSR(s)          EDITOR_PSR(s)
#define EDITOR_PSR(s)           #s 

#define JOIN_PATH(lhs, rhs)     (lhs + "/" + "rhs")
#define GET_ABSOLUTE_PATH(rhs)  (EDITOR_PXSR(VE_ROOT_DIR) + rhs)

#define MAIN_LAYER_NAME         "Main-Layer"
#define SCENE_LAYER_NAME        "Scene-Layer"
#define SE_LAYER_NAME           "ShaderEditor-Layer"

#include <string>
#include <iostream>
#include <vector>

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_glfw.h>
#include <imgui/backend/imgui_impl_opengl3.h>
#include <imgui/imGuiFileDialog.h>
#include <imgui/imgui_internal.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/zmo/ImGuizmo.h>

#include "Core/include/core_event_sys.hpp"

#define EVENT_CALL_BACK private

namespace evergarden {

    static std::string get_abs_path(std::string rhs) {
        return EDITOR_PXSR(VE_ROOT_DIR) + rhs;
    }

} //! namespace evergarden

#endif //! H_EDITOR_BASE_