/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/src/renderer_buffer.cpp
 * @brief   A API package for buffer class.
 */

#include "../include/render_buffer.hpp"

namespace evergarden {

    uint32_t InnerDataTypeSize(InnerDataType type) {
        switch (type) {
            case InnerDataType::Float:			return 4;
            case InnerDataType::Vec2f:			return 4 * 2;
            case InnerDataType::Vec3f:			return 4 * 3;
            case InnerDataType::Vec4f:			return 4 * 4;
            case InnerDataType::Mat3:			return 4 * 3 * 3;
            case InnerDataType::Mat4:			return 4 * 4 * 4;
            case InnerDataType::Int:			return 4;
            case InnerDataType::Vec2d:			return 4 * 2;
            case InnerDataType::Vec3d:			return 4 * 3;
            case InnerDataType::Vec4d:			return 4 * 4;
            case InnerDataType::Bool:			return 1;
            case InnerDataType::None:
                break;
        }
        return 0;
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size) {
        return std::make_shared<VertexBuffer_GL>(vertices, size);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        return std::make_shared<VertexBuffer_GL>(size);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size) {
        return std::make_shared<IndexBuffer_GL>(indices, size);
    }

    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferElement &elem) {
        return std::make_shared<FrameBuffer_GL>(elem);
    }

    std::shared_ptr<VertexArray> VertexArray::Create() {
        return std::make_shared<VertexArray_GL>();
    }


    // Opengl based implementation below.
    VertexBuffer_GL::VertexBuffer_GL(uint32_t size) {
        glCreateBuffers(1, &c_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, c_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer_GL::VertexBuffer_GL(float *vertices, uint32_t size) {
        glCreateBuffers(1, &c_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, c_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void VertexBuffer_GL::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, c_RendererID);
    }

    void VertexBuffer_GL::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    FrameBuffer_GL::FrameBuffer_GL(const FrameBufferElement &elem):c_Element(elem) {
        Invalidate();
    }

    FrameBuffer_GL::~FrameBuffer_GL() {
        glDeleteFramebuffers(1, &c_RendererID);
        glDeleteTextures(1, &c_ColorAttachment);
        glDeleteTextures(1, &c_DepthAttachment);
    }

    void FrameBuffer_GL::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, c_RendererID);
        glViewport(0, 0, c_Element.Width, c_Element.Height);
    }

    void FrameBuffer_GL::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer_GL::Resize(uint32_t width, uint32_t height) {
        c_Element.Width = width;
        c_Element.Height = height;
        Invalidate();
    }

    void FrameBuffer_GL::Invalidate() {
        if (c_RendererID) {
            glDeleteFramebuffers(1, &c_RendererID);
            glDeleteTextures(1, &c_ColorAttachment);
            glDeleteTextures(1, &c_DepthAttachment);
        }

        glCreateFramebuffers(1, &c_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, c_RendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &c_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, c_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, c_Element.Width, c_Element.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, c_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &c_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, c_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, c_Element.Width, c_Element.Height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, c_DepthAttachment, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // Opengl Vertex Array pac
    GLenum InnerDataType2GLType(InnerDataType type) {
        switch (type) {
            case InnerDataType::Float:
            case InnerDataType::Vec2f:
            case InnerDataType::Vec3f:
            case InnerDataType::Vec4f:
            case InnerDataType::Mat3:
            case InnerDataType::Mat4:			return GL_FLOAT;
            case InnerDataType::Int:
            case InnerDataType::Vec2d:
            case InnerDataType::Vec3d:
            case InnerDataType::Vec4d:			return GL_INT;
            case InnerDataType::Bool:			return GL_BOOL;
            case InnerDataType::None:
                break;
        }
        return 0;
    }

    VertexArray_GL::VertexArray_GL() {
        glCreateVertexArrays(1, &c_RendererID);
    }

    VertexArray_GL::~VertexArray_GL() {
        glDeleteVertexArrays(1, &c_RendererID);
    }

    void VertexArray_GL::Bind() const {
        glBindVertexArray(c_RendererID);
    }

    void VertexArray_GL::Unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray_GL::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
        glBindVertexArray(c_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        for (const auto& element : vertexBuffer->GetLayout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.GetComponentCnt(),
                                  InnerDataType2GLType(element.c_Type),
                                  element.c_Normalized ? GL_TRUE : GL_FALSE,
                                  vertexBuffer->GetLayout().GetStride(),
                                  (const void*)(element.c_Offset));
            index++;
        }
        c_VertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray_GL::AddIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
        glBindVertexArray(c_RendererID);
        indexBuffer->Bind();
        c_IndexBuffer = indexBuffer;
    }

    IndexBuffer_GL::IndexBuffer_GL(uint32_t *indices, uint32_t cnt):c_cnt(cnt) {
        glCreateBuffers(1, &c_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cnt * sizeof(cnt), indices, GL_STATIC_DRAW);
    }

    void IndexBuffer_GL::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_RendererID);
    }

    void IndexBuffer_GL::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} //! evergarden
