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

#include "Editor/include/editor_core.hpp"
#include "Core/include/core_time.hpp"

namespace evergarden {

    class __declspec(dllexport) App {
    public:
        App(std::string window_name = "Evergarden");
        ~App();

    public:
        void                        run();
        void                        onEvent(Event &e);
        void                        pushLayer(Layer* rhs);
        void                        popLayer(Layer* rhs);
        bool                        shutdown();

    public:
        static App*                 create(); 
        static App*                 getInstance() { return Instance; };

    public:
        static App*                 Instance;
    
    private:
        std::shared_ptr<MainWindow> m_window;
        ImGuiLayer*                 m_imgui_initial_layer;
        LayerStack                  m_layer_stack;
        bool                        m_window_run = true;
        bool                        m_render_run = true;
        bool                        m_window_minimize = false;
        float                       m_last_frame_time_cnt = 0.f;
        TimeFrame                   m_time_frame;

    EVENT_CALL_BACK:
        ///< This call back just for normal Application Event.
        bool __on_app_window_resize__(WindowResizeEvent &e);
        bool __on_app_window_close__(WindowCloseEvent &e);
    };

}

#endif //! H_APPLICATION

