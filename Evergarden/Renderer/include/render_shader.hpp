/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_shader.hpp
 * @brief   A API package for shader class.
 */

#ifndef H_RENDER_SHADER
#define H_RENDER_SHADER

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include <glm/glm.hpp>

namespace evergarden {

    enum class ShaderType{
        None        = 0,
        Vertex      = 1,
        Fragment    = 2
    };

    struct ShaderProgramSource {
        std::string VertexSource{};
        std::string FragmentSource{};
    };

    ShaderProgramSource ParseShader(const std::string& FilePath);
    unsigned int        CompileShader(unsigned int type, const std::string& source);
    unsigned int        CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    class Shader{
    public:
        Shader()    = default;
        ~Shader()   = default;

        virtual void    bind()                  const = 0;
        virtual void    unbind()                const = 0;
        virtual const   std::string GetName()   const = 0;

        // below for uniform setting
        virtual void setInt(const std::string& name, int value)                             const = 0;
        virtual void setFloat(const std::string& name, float value)                         const = 0;
        virtual void setVec2(const std::string& name, const glm::vec2& value)               const = 0;
        virtual void setVec2(const std::string& name, float x, float y)                     const = 0;
        virtual void setVec3(const std::string& name, const glm::vec3& value)               const = 0;
        virtual void setVec3(const std::string& name, float x, float y, float z)            const = 0;
        virtual void setVec4(const std::string& name, const glm::vec4& value)               const = 0;
        virtual void setVec4(const std::string& name, float x, float y, float z, float w)   const = 0;
        virtual void setMat2(const std::string& name, const glm::mat2& mat)                 const = 0;
        virtual void setMat3(const std::string& name, const glm::mat3& mat)                 const = 0;
        virtual void setMat4(const std::string& name, const glm::mat4& mat)                 const = 0;
        virtual void setBool(const std::string& name, bool value)                           const = 0;

        static std::shared_ptr<Shader> create(const std::string& all_src);
        static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);

    protected:
        std::string m_name;
    };

    class Shader_OpenGL: public Shader{
    public:
        Shader_OpenGL(const std::string& all_src);
        Shader_OpenGL(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);

        ~Shader_OpenGL();

        // below for function override
        void    bind()                  const override;
        void    unbind()                const override;
        const   std::string GetName()   const override { return m_name; }

        // below for uniform setting
        void setInt(const std::string& name, int value)                             const override;
        void setFloat(const std::string& name, float value)                         const override;
        void setVec2(const std::string& name, const glm::vec2& value)               const override;
        void setVec2(const std::string& name, float x, float y)                     const override;
        void setVec3(const std::string& name, const glm::vec3& value)               const override;
        void setVec3(const std::string& name, float x, float y, float z)            const override;
        void setVec4(const std::string& name, const glm::vec4& value)               const override;
        void setVec4(const std::string& name, float x, float y, float z, float w)   const override;
        void setMat2(const std::string& name, const glm::mat2& mat)                 const override;
        void setMat3(const std::string& name, const glm::mat3& mat)                 const override;
        void setMat4(const std::string& name, const glm::mat4& mat)                 const override;
        void setBool(const std::string& name, bool value)                           const override;

    private:
        uint32_t m_id = 0;
    };

    class ShaderStack{
    public:
        void Add(const std::shared_ptr<Shader>& rhs);
        void Add(const std::string& rhs_name, const std::shared_ptr<Shader>& rhs_shader);

        std::shared_ptr<Shader> Load(const std::string& rhs_filepath);
        std::shared_ptr<Shader> Load(const std::string& rhs_name, const std::string& rhs_filepath);

        std::shared_ptr<Shader> Get(const std::string& rhs_name);

        bool Exists(const std::string& rhs_name) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    };

} //! namespace evergarden

#endif //! H_RENDER_SHADER
