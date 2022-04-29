/*!
 * @author  chenghua.wang
 * @file    Evergarden/App/include/Application.hpp
 * @brief   Application class manage all engine resource.
 */

#ifndef H_APPLICATION
#define H_APPLICATION

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "Editor/include/app_window.hpp"
#include "Editor/include/editor_layer.hpp"

namespace evergarden {

    class App {
    public:
        App(std::string window_name);
        ~App();

    public:
        void run();
        void onEvent(Event &e);
        void pushLayer(Layer* rhs);
        void popLayer(Layer* rhs);
        void shutdown();

    EVENT_CALL_BACK:
        ///< This call back just for normal Application Event.
        bool __on_app_window_resize__(WindowCloseEvent &e);
        bool __on_app_window_close__(WindowCloseEvent &e);
    
    private:
        std::shared_ptr<MainWindow> m_window;
        static App*                 Instance;
        ImGuiLayer*                 m_imgui_initial_layer;
        LayerStack                  m_layer_stack;
        bool                        m_window_run = true;
        bool                        m_render_run = true;
        bool                        m_window_minimize = true;
        float                       m_last_frame_time_cnt = 0.f;
    };

}

#endif //! H_APPLICATION

