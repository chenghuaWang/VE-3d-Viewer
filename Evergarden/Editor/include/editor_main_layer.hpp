/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_main_layer.hpp
 * @brief   Main Layer contain multi layers.
 */

#ifndef H_EDITOR_MAIN_LAYER
#define H_EDITOR_MAIN_LAYER

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "editor_base.hpp"
#include "editor_layer.hpp"

namespace evergarden {

    class Layer_Main: public Layer {
    public:
        Layer_Main();
        ~Layer_Main() = default;
    
    public:
        void onAttach()         override {};
        void onDetach()         override {};
        void onUpdate()         override {};
        void onGuiRender()      override;
        void onEvent(Event& e)  override {};

    private:
        

    };

}//! evergarden

#endif //! H_EDITOR_MAIN_LAYER