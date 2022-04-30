/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/src/renderer_renderer.cpp
 * @brief   A API package for renderer class.
 */

#include "../include/render_rednerer.hpp"

namespace evergarden {

    Renderer::SceneData* Renderer::c_SceneData = new Renderer::SceneData;

    void Renderer::__init__() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    void Renderer::__shutdown__() {
        return ;
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        glViewport(0, 0, (int)width, (int)height);
    }

    void Renderer::beginScene(Camera &camera) {
        c_SceneData->ViewProjectionMatrix = camera._ViewProjectionMatrix();
    }

    void Renderer::endScene() {

    }

    void Renderer::__submit__(const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Shader> &shader,
                          const glm::mat4 &transform) {
        shader->bind();
        shader->setMat4("cpu_ViewProjection", c_SceneData->ViewProjectionMatrix);
        shader->setMat4("cpu_Transform", transform);

        vertexArray->Bind();

        glDrawElements(GL_TRIANGLES, (int)vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
