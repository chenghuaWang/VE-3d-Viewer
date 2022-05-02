/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_renderer.hpp
 * @brief   A API package for renderer class.
 */

#ifndef H_RENDER_RENDERER
#define H_RENDER_RENDERER

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include "render_camera.hpp"
#include "render_buffer.hpp"
#include "render_shader.hpp"

namespace evergarden {

    class Renderer {
    public:
        static void __init__();
        static void __shutdown__();

        static void OnWindowResize(uint32_t width, uint32_t height); ///< not a call-back function!!!

        static void beginScene(Camera& camera);
        static void endScene();

        static void __submit__(const std::shared_ptr<VertexArray>& vertexArray,
                                const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

    private:
        
        struct SceneData {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* c_SceneData;
    };

} //! evergarden

#endif //! H_RENDER_RENDERER