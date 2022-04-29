/*!
 * @author  chenghua.wang
 * @file    Evergarden/App/include/app_window.hpp
 * @brief   Provides a multi-platform MainWindow.
 */

#ifndef H_APP_WINDOW
#define H_APP_WINDOW

#ifdef WIN32
#pragma once
#endif //! WIN32

#include <string>
#include <functional>
#include <memory>

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

#include "Core/include/core_event_sys.hpp"
#include "Renderer/include/render_context.hpp"

#define _APP_START_REGION_  { 

#define _APP_END_REGION_    }

namespace evergarden {

    /*!
     * @brief A Window structure for MainWindow. Record the size of window and its name.
     */
    class MainWindowPayload {
    public:
        MainWindowPayload(): m_W(1920), m_H(1080), m_title("Evergarden") {};
        MainWindowPayload(uint32_t w, uint32_t h, std::string &title): m_W(w), m_H(h), m_title(title) {};
        ~MainWindowPayload() = default;

    public:
        uint32_t        m_W, m_H;
        std::string     m_title;

    };


    /*!
     * @brief Class for application main window.
     *
     * @details just like a win32-api window's handler. This class provide
     * a simple approach for Main window. All layers will be create on this
     * window, and all <platform related event> will be get from this class
     * . I use a call back function to let application can react from this 
     * Main window class.
     * 
     * Application class will use this Mainwindow class to create desktop app
     * ui context, and organize layer on it.
     */
    class MainWindow {
    public:
        MainWindow() = default;
        virtual ~MainWindow() = default;
        
    public:
        virtual void        onUpdate() = 0;
        virtual void        setEventCallBack(std::function<void(Event&)> &e) = 0;
        virtual void        setVSync(bool enable) = 0;
        virtual bool        isVSync(bool enable) = 0;
        virtual void*       getPlatformWindow() = 0; ///< to get the platform based window. Such as glfw, DX12, WinAPI
        virtual uint32_t    W() = 0;
        virtual uint32_t    H() = 0;
        virtual void        getSize(uint32_t &w, uint32_t &h) = 0;

        static  std::shared_ptr<MainWindow> create(MainWindowPayload &rhs = MainWindowPayload());
    
    };

} //! evergarden


///< Below is opengl implement of MainWindow
namespace evergarden {

    struct __opengl_callback_data__ {
        std::string                 m_titile;
        uint32_t                    m_w;
        uint32_t                    m_h;
        std::function<void(Event&)> m_event_call_back_;
        bool                        m_Vsync;
    };

    class MainWindow_OpenGL: public MainWindow {
    public:
        MainWindow_OpenGL(MainWindowPayload &payload);
        ~MainWindow_OpenGL()                                        override;

    public:
        void        onUpdate()                                      override;
        void        setEventCallBack(std::function<void(Event&)> &e)override { m__opengl_callback_data_.m_event_call_back_=e; };
        void        setVSync(bool enable)                           override;
        bool        isVSync(bool enable)                            override { return m__opengl_callback_data_.m_Vsync; };
        void*       getPlatformWindow()                             override { return m_handle; };
        uint32_t    W()                                             override { return m__opengl_callback_data_.m_w; };
        uint32_t    H()                                             override { return m__opengl_callback_data_.m_h; };
        void        getSize(uint32_t &w, uint32_t &h)               override { w=W(); h=H(); };

    private:
        virtual     void __init__(MainWindowPayload &payload);
        virtual     void __shutdown__();
    
    private:
        GLFWwindow*                 m_handle = nullptr;
        __opengl_callback_data__    m__opengl_callback_data_; ///< This data struct is for OpenGL window callback to use.
        std::shared_ptr<Context>    m_context;///< The main window will maintain the context of low-lever glfw opengl context.

    };

}

#endif //! H_APP_WINDOW
