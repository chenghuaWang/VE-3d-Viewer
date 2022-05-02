/*!
 * @author  chenghua.wang
 * @file    Evergarden/Editor/include/editor_hia.hpp
 * @brief   Main Layer contain multi layers.
 */

#ifndef H_EDITOR_HIA
#define H_EDITOR_HIA

#ifdef WIN32
#pragma once
#endif //! WIN32

#include "editor_base.hpp"
#include "editor_scene_layer.hpp"
#include "Renderer/include/render_entity.hpp"
#include <entt/entt.hpp>

namespace evergarden {

    class Hia {
    public:
        static void __on_show_hia__(bool *is_open, Layer_Scene *scene);
        static void __on__tree__(Entity node);
        static void __on_remove__(Entity &node_noneed);
    };

} //! namespace evergarden

#endif //! H_EDITOR_HIA
