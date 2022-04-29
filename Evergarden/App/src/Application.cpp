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
    }

    App::~App() {
        
    }

}
