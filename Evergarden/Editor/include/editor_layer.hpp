/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_layer.hpp
 * @brief   The most basic class for all layers.
 */

#ifndef H_EDITOR_LAYER
#define H_EDITOR_LAYER

#ifdef WIN32
#pragma once
#endif //! WIN32

#include "editor_base.hpp"
#include "app_window.hpp"

namespace evergarden {

    /*!
     * @brief class for Layer abstract.
     *
     * @details Provide a abstract for Imgui Layer. And add multi
     * blocking-events method for Layer to react the event send by
     * application our other layers.
     */
    class __declspec(dllexport) Layer {
    public:
        Layer(std::string layer_name): m_layer_name_(layer_name) {};
        virtual ~Layer() = default;

    public:
        void __setIdxInStack__(int x) { m_idx_in_stack = x; };
        virtual void onAttach(){};
        virtual void onDetach(){};
        virtual void onUpdate(){};
        virtual void onGuiRender(){};
        virtual void onEvent(Event &e){};

    public:
        std::string  _layer_name_() {return m_layer_name_; };

    private:
        virtual bool __on_key_typed__(KeyTypedEvent &e) { return false; };
        virtual bool __on_key_down__(KeyPressedEvent &e) { return false; };
        virtual bool __on_key_release__(KeyReleasedEvent &e) { return false; };
        virtual bool __on_mouse_move__(MouseMovedEvent &e) { return false; };
        virtual bool __on_mouse_scroll__(MouseScrolledEvent &e) { return false; };
        virtual bool __on_mouse_down__(MouseButtonPressedEvent &e) { return false; };///< Mouse button pressed
        virtual bool __on_mouse_release__(MouseButtonReleasedEvent &e) { return false; };///< Mouse button released
        // TODO ADD Layer info broadcast. Application Event don't need in layer right now.

    private:
        int             m_idx_in_stack;
        std::string     m_layer_name_;

    }; //! class Layer

    class __declspec(dllexport) LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();

    public:
        void pushLayer(Layer* la);
        void popLayer(Layer* la);

    public:
        std::vector<Layer*>::iterator begin()   { return m_stack.begin(); }
        std::vector<Layer*>::iterator end()     { return m_stack.end(); }
    
    private:
        size_t                 m_cur_idx = 0;
        std::vector<Layer*>    m_stack;
    }; //! class LayerStack

}//! namespace evergarden


namespace evergarden {
 
    class __declspec(dllexport) ImGuiLayer: public Layer {
    public:
        ImGuiLayer(std::shared_ptr<MainWindow> window);
        ~ImGuiLayer() override;

    public:
        void onAttach() override;
        void onDetach() override;
        void begin();
        void end();
        void _setEventBlock_(bool enable) { m_event_block_enable = enable; };

    private:
        float                       m_time_update = 0;
        bool                        m_event_block_enable = true;
        std::shared_ptr<MainWindow> m_window;///< Layer hold the father window. It's window class holds the handle.
    };

}

#endif //! H_EDITOR_LAYER
