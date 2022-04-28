/*!
 * @author  chenghua.wang
 * @file    Evergarden/Renderer/include/render_contex.hpp
 * @brief   A contex api for Render.
 * 
 * @details We packagged OpenGL API in this Class-like interface.
 */

#ifndef H_REDNER_CONTEXT
#define H_RENDER_CONTEXT

#ifdef WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"

namespace evergarden {

    class Context {
    public:
        virtual void init() = 0;
        virtual void SwapBuffer() = 0;

        static  std::shared_ptr<Context> create(GLFWwindow *handle);
    };

}

namespace evergarden {

    class Context_OpenGL: public Context {
    public:
        Context_OpenGL(GLFWwindow *handle); ///< don not use explict.

    public:
        void init()         override; 
        void SwapBuffer()   override;

    private:
        GLFWwindow  *m_handle;

    };

}

#endif //! H_RENDER_CONTEXT