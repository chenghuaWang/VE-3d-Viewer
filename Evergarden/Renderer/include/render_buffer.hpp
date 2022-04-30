/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_buffer.hpp
 * @brief   A API package for buffer class.
 */

#ifndef H_RENDER_BUFFER
#define H_REDNER_BUFFER

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace evergarden {

    struct Vertex {
        ~Vertex() = default;

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        bool operator== (const Vertex& v) const {
            return Position == v.Position && TexCoords == v.TexCoords && Normal == v.Normal;
        };
    };

    struct Triangle {
        uint32_t id1;
        uint32_t id2;
        uint32_t id3;
    };

    enum class InnerDataType {
        None    = 0,
        Float   = 1,
        Vec2f   = 2,
        Vec3f   = 3,
        Vec4f   = 4,
        Int     = 5,
        Vec2d   = 6,
        Vec3d   = 7,
        Vec4d   = 8,
        Mat3    = 9,
        Mat4    = 10,
        Bool    = 11
    };

    uint32_t InnerDataTypeSize(InnerDataType type);

    class BufferElement {
    public:
        BufferElement() = default;
        BufferElement(InnerDataType type, const std::string& name, bool normalized = false)
                :c_Type(type), c_Name(name), c_Size(InnerDataTypeSize(type)), c_Offset(0), c_Normalized(normalized){}
    public:
        uint32_t GetComponentCnt() const {
            switch (c_Type) {
                case InnerDataType::Float:			return 1;
                case InnerDataType::Vec2f:			return 2;
                case InnerDataType::Vec3f:			return 3;
                case InnerDataType::Vec4f:			return 4;
                case InnerDataType::Mat3:			return 3 * 3;
                case InnerDataType::Mat4:			return 4 * 4;
                case InnerDataType::Int:			return 1;
                case InnerDataType::Vec2d:			return 2;
                case InnerDataType::Vec3d:			return 3;
                case InnerDataType::Vec4d:			return 4;
                case InnerDataType::Bool:			return 1;
                case InnerDataType::None:
                    break;
            }
            return 0;
        }

    public:
        uint32_t c_Size;
        uint32_t c_Offset;
        InnerDataType c_Type;
        bool c_Normalized;
        std::string c_Name;
    };

    class FrameBufferElement {
    public:
        FrameBufferElement() = default;
        FrameBufferElement(uint32_t w, uint32_t h):Width(w), Height(h) {};
        ~FrameBufferElement() = default;

    public:
        uint32_t Width{}, Height{};
        uint32_t Samples = 1;
        bool SwapChainTarget = false;
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements):c_Elements(elements){
            CalculateOffsetAndStride();
        }

        inline uint32_t GetStride() const { return c_Stride; }
        inline const std::vector<BufferElement>& GetElements() const { return c_Elements; }


        // That bunch of functions make this class works like an iterable item.
        std::vector<BufferElement>::iterator begin()                { return c_Elements.begin(); }
        std::vector<BufferElement>::iterator end()                  { return c_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const    { return c_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const      { return c_Elements.end(); }

    private:
        void CalculateOffsetAndStride() {
            uint32_t offset = 0;
            c_Stride = 0;
            for (auto& element : c_Elements) {
                element.c_Offset = offset;
                offset += element.c_Size;
                c_Stride += element.c_Size;
            }
        }

    private:
        uint32_t c_Stride = 0;
        std::vector<BufferElement> c_Elements;
    };


    // Basic Class. Need use Platform based functions to override it.
    // Those functions work just like the Texture class did.
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
        static std::shared_ptr<VertexBuffer> Create(uint32_t size);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;

        static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t size);
    };

    class FrameBuffer {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual uint32_t GetColorAttachmentID() const = 0;
        virtual FrameBufferElement& GetElement() = 0;
        virtual const FrameBufferElement& GetElement() const = 0;
        static std::shared_ptr<FrameBuffer> Create(const FrameBufferElement& spec);
    };

    class VertexArray {
    public:
        virtual ~VertexArray() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        static std::shared_ptr<VertexArray> Create();
    };

    // Opengl platform implementation.
    class VertexBuffer_GL: public VertexBuffer {
    public:
        explicit VertexBuffer_GL(uint32_t size);
        VertexBuffer_GL(float* vertices, uint32_t size);
        ~VertexBuffer_GL() override = default;

        void Bind() const override;
        void Unbind() const override;

        void SetLayout(const BufferLayout& layout) override { c_Layout = layout; }
        const BufferLayout& GetLayout() const override { return c_Layout; }

    private:
        uint32_t c_RendererID = 0;
        BufferLayout c_Layout;
    };

    class IndexBuffer_GL: public IndexBuffer {
    public:
        IndexBuffer_GL(uint32_t *indices, uint32_t cnt);
        ~IndexBuffer_GL() override = default;

        void Bind() const override;
        void Unbind() const override;

        uint32_t GetCount() const override { return c_cnt; };
    private:
        uint32_t c_RendererID = 0;
        uint32_t c_cnt;
    };

    class FrameBuffer_GL: public FrameBuffer {
    public:
        FrameBuffer_GL(const FrameBufferElement& elem);
        virtual ~FrameBuffer_GL();

        void Bind() override;
        void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        uint32_t GetColorAttachmentID() const override { return c_ColorAttachment; }

        FrameBufferElement& GetElement() override { return c_Element; }
        const FrameBufferElement& GetElement() const override { return c_Element; }

    public:
        void Invalidate();

    private:
        uint32_t c_RendererID = 0;
        uint32_t c_ColorAttachment = 0, c_DepthAttachment = 0;
        FrameBufferElement c_Element;
    };

    GLenum InnerDataType2GLType(InnerDataType type);

    class VertexArray_GL: public VertexArray {
    public:
        VertexArray_GL();
        ~VertexArray_GL() override;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return c_VertexBuffers; }
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return c_IndexBuffer; }

    private:
        uint32_t c_RendererID = 0;
        std::vector<std::shared_ptr<VertexBuffer>> c_VertexBuffers;
        std::shared_ptr<IndexBuffer> c_IndexBuffer;
    };

} //! namespace evergarden


namespace std {
    template<> struct hash<evergarden::Vertex> {
        size_t operator()(evergarden::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.Position) ^
                     (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.TexCoords) << 1);
        }
    };
} //! namespace std


#endif //! H_REDNER_BUFFER
