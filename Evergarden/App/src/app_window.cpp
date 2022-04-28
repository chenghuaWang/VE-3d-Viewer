/*!
 * @author  chenghua.wang
 * @file    Evergarden/App/include/app_window.hpp
 * @brief   Provides a multi-platform MainWindow.
 */

#include "../include/app_window.hpp"

namespace evergarden {

    static int _out_Window_cnt_ = 0;

    std::shared_ptr<MainWindow> MainWindow::create(MainWindowPayload &rhs = MainWindowPayload()) {
        return std::make_shared<MainWindow_OpenGL>(rhs);
    }

    MainWindow_OpenGL::MainWindow_OpenGL(MainWindowPayload &payload) {
        __init__(payload);
    }

    MainWindow_OpenGL::~MainWindow_OpenGL() {
        __shutdown__();
    }

    void MainWindow_OpenGL::onUpdate() {

    }

    void MainWindow_OpenGL::setEventCallBack(std::function<void(Event&)>) {

    }

    void MainWindow_OpenGL::setVSync(bool enable) {
        if (enable) glfwSwapInterval(1);
		else        glfwSwapInterval(0);
    }

    void MainWindow_OpenGL::__init__(MainWindowPayload &payload) {
        m__opengl_callback_data_.m_titile   = payload.m_title;
        m__opengl_callback_data_.m_w        = payload.m_W;
        m__opengl_callback_data_.m_h        = payload.m_H;
        
        if (_out_Window_cnt_ == 0) {
            glfwInit();
        }

        _APP_START_REGION_
        m_handle = glfwCreateWindow((int)payload.m_W, (int)payload.m_H, payload.m_title.c_str(), nullptr, nullptr);
	    ++ _out_Window_cnt_;
        _APP_END_REGION_

        m_context = Context::create(m_handle);
        m_context->init();

        glfwSetWindowUserPointer(m_handle, &m__opengl_callback_data_);
		setVSync(true);

        // Set GLFW callbacks
		glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* window, int width, int height) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);
			data.m_w = width;
			data.m_h = height;

			WindowResizeEvent event(width, height);
			data.m_event_call_back_(event);
		});

		glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* window) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.m_event_call_back_(event);
		});

		glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.m_event_call_back_(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.m_event_call_back_(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.m_event_call_back_(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_handle, [](GLFWwindow* window, unsigned int keycode) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.m_event_call_back_(event);
		});

		glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.m_event_call_back_(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.m_event_call_back_(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_handle, [](GLFWwindow* window, double xOffset, double yOffset) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.m_event_call_back_(event);
		});

		glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double xPos, double yPos) {
			__opengl_callback_data__& data = *(__opengl_callback_data__*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.m_event_call_back_(event);
		});

    }

    void MainWindow_OpenGL::__shutdown__() {

    }

}
