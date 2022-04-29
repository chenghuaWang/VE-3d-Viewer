/*!
 * @author  chenghua.wang
 * @file    Evergarden/Core/src/core_time.cpp
 * @brief   A time class for more fluent render
 */

#include "../include/core_time.hpp"

namespace evergarden {

    void TimeFrame::update(float lastFrameTime) {
        m_time = (float)glfwGetTime() - lastFrameTime;
    };

}
