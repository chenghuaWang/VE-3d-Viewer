/*!
 * @author  chenghua.wang
 * @file    Evergarden/App/src/Application.cpp
 * @brief   Application class manage all engine resource.
 */

#include "../include/Application.hpp"

namespace evergarden {

    App* App::Instance = nullptr;

    App::App(std::string window_name) {
        Instance = this;
        m_time_frame.clear();

        MainWindowPayload pm(window_name);
        m_window = std::shared_ptr<MainWindow>(MainWindow::create(pm));
        m_window->setEventCallBack(std::bind(&App::onEvent, this, std::placeholders::_1));

        // TODO init renderer

        m_imgui_initial_layer = new ImGuiLayer(m_window);
        pushLayer(m_imgui_initial_layer);
    }

    App::~App() {
        // TODO shutdown renderer machine

        // Layer pointer will be released by RAII of LayerStack

    }

    void App::run() {
        m_time_frame.clear();
        while (m_window_run) {
            m_time_frame.update(m_last_frame_time_cnt);
            m_last_frame_time_cnt += m_time_frame.getSeconds();

            if (!m_window_minimize) {
                for (Layer* layer : m_layer_stack) {
                    layer->onUpdate();
                    layer->onUpdate(m_time_frame);
                }
                m_imgui_initial_layer->begin();
                {
                    for (Layer* layer : m_layer_stack) {
                        layer->onGuiRender();
                    }
                }
                m_imgui_initial_layer->end();
            }
            m_window->onUpdate();
        }
    }

    void App::onEvent(Event &e) {
        ///< Process current event in application.
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&App::__on_app_window_close__, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&App::__on_app_window_resize__, this ,std::placeholders::_1));

        ///< Process all event in Layers.
        for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();) {
            (*--it)->onEvent(e);
            if (e.m_Handled) break;
        }
    }

    void App::pushLayer(Layer* rhs) {
        m_layer_stack.pushLayer(rhs);
        rhs->onAttach();
    }

    void App::popLayer(Layer* rhs) {
        m_layer_stack.popLayer(rhs);
        rhs->onDetach();
    }

    bool App::shutdown() {
        m_window_run = false;
        return true;
    }

    bool App::__on_app_window_resize__(WindowResizeEvent &e) {
        ///< This fucntion is call back.
         if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_window_minimize = true;
            return false;
        }
        m_window_minimize = false;
        return true;
    }

    bool App::__on_app_window_close__(WindowCloseEvent &e) {
        ///< This fucntion is call back.
        shutdown();
        return true;
    }

    App* App::create() {
        return new App();
    }

}
