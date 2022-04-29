/*!
 * @author  chenghua.wang
 * @file    Evergarden/Core/include/core_time.hpp
 * @brief   A time class for more fluent render
 */

#ifndef H_CORE_TIME
#define H_CORE_TIME

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include <GLFW/glfw3.h>

namespace evergarden {

    class TimeFrame {
    public:
        TimeFrame(float time = 0.0f): m_time(time) {};
        operator float() const { return m_time; }

        float   getSeconds() const                  { return m_time; };
        float   getMilliseconds() const             { return m_time * 1000.0f; };
        void    update(float lastFrameTime = 0.0f);
        void    clear()                             { m_time = 0.f; };

    private:
        float m_time;
    };

} //! namespace evergarden

#endif //! H_CORE_TIME
