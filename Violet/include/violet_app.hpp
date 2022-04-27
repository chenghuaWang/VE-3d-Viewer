/*!
 *@author   chenghua.wang
 *@file     Violet/include/violet_app.hpp
 *@brief    A simple orgnized launcher class.
 */

#ifndef H_WIOLET_HPP_
#define H_WIOLET_HPP_

#ifdef WIN32
#pragma once
#endif

#include <sys/stat.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_glfw.h>
#include <imgui/backend/imgui_impl_opengl3.h>
#include <imgui/imGuiFileDialog.h>
#include <imgui/imgui_internal.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <json11/json11.hpp>

#include <stb_image/stb_image.h>

#ifdef WIN32
#define ASSERT(x) {if(!(x)) __debugbreak();}
#else
#define ASSERT(x) {if(!(x)) asm("int $3");}
#endif

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#define VE_XSR(s) VE_SR(s)
#define VE_SR(s) #s

#define MAGIC_KEY       "GB4706"

#define RENDER_JSON_KEY "RenderDll"
#define EDITOR_JSON_KEY "EditorDll"
#define CORE_JSON_KEY   "CoreDll"

#ifdef WIN32
#define RENDER_DLL_NAME "Evergarden_Render.dll"
#define EDITOR_DLL_NAME "Evergarden_Editor.dll"
#define CORE_DLL_NAME   "Evergarden_Core.dll"
#else
#define RENDER_DLL_NAME "Evergarden_Render.so"
#define EDITOR_DLL_NAME "Evergarden_Editor.so"
#define CORE_DLL_NAME   "Evergarden_Core.so"
#endif

#define HELP_INFO       "Under MIT Licence \
\n Created with love. \n chenghua.wang"

using namespace json11;

namespace violet {

    enum class DLL_TYPE {
        None = 0,
        Render,
        Editor,
        Core,
    };

    class App {
    public:
        App();
        ~App();
    
    public:
        bool        InitConetx();
        void        onUpdate();
        void        run();

    private:
        bool        _check_file_(std::vector<std::string> rhs);
        bool        _file_exists_(std::string &rhs);
        void        _exec_evergarden_(std::string &rhs);
        void        _exit_();
        std::string _get_absolute_path_(std::string &rhs);
        std::string _join_path_(std::string &lhs, std::string rhs);
        std::vector<std::string> App::_parse_json_(std::string &rhs);

    private:
        static void _help_window_();

    private:
        bool        m_window_on;
        std::string m_scene_path;
        GLFWwindow* m_window_handle;
        ImTextureID m_bk_image;
        Json        m_json;
        bool        m_progress_window_on;
        bool        m_choose_window_on;
        static float m_buffer_bar_val;
    };

    static App* CreateApp() {
        return new App();
    }

    static void glfw_error_callback(int error, const char* description) {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

} // namespace violet

#endif //! H_WIOLET_HPP_
