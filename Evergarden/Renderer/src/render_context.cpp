/*!
 * @author  chenghua.wang
 * @file    Evergarden/Renderer/src/render_base.cpp
 * @brief   A contex api for Render.
 * 
 * @details We packagged OpenGL API in this Class-like interface.
 */

#include "../include/render_context.hpp"

namespace evergarden {

    std::shared_ptr<Context> Context::create(GLFWwindow *handle) {
        return std::make_shared<Context_OpenGL>(handle);
    }

    Context_OpenGL::Context_OpenGL(GLFWwindow *handle): m_handle(handle) {
        if (handle == nullptr) {
            std::cout << "Err: When init opengl context, find window's handle is null!" << std::endl;
        }
    }

    void Context_OpenGL::init() {
        glfwMakeContextCurrent(m_handle);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void Context_OpenGL::SwapBuffer() {
        glfwSwapBuffers(m_handle);
    }

}
