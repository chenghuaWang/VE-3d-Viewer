/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/src/renderer_shader.cpp
 * @brief   A API package for shader class.
 */

#include "../include/render_shader.hpp"

namespace evergarden {

     /* below for static global functions */
    ShaderProgramSource ParseShader(const std::string& FilePath){
        std::ifstream stream(FilePath);
        std::string line;
        std::stringstream ss[2];
        ShaderType Type = ShaderType::None;
        while (std::getline(stream, line)) {
            if (line.find("##shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    Type = ShaderType::Vertex;
                }
                else if (line.find("fragment") != std::string::npos) {
                    Type = ShaderType::Fragment;
                }
            } else {
                ss[(int)Type - 1] << line << "\n";
            }
        }
        return { ss[0].str(), ss[1].str() };
    }

    unsigned int CompileShader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        GLCall(glShaderSource(id, 1, &src, nullptr));
        GLCall(glCompileShader(id));

        int result;
        GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE) {
            int length;
            GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length););
            char* msg = (char*)malloc(length * sizeof(char)); //_malloca on win32 platform
            GLCall(glGetShaderInfoLog(id, length, &length, msg));
            std::cout << "Failed to compile :" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
            std::cout << msg << std::endl;
            GLCall(glDeleteShader(id););
        }
        return id;
    }

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    /* below for Shader */
    std::shared_ptr<Shader>
    Shader::create(const std::string &all_src) {
        return std::make_shared<Shader_OpenGL>(all_src);
    }

    std::shared_ptr<Shader>
    Shader::create(const std::string &name, const std::string &vertex_src, const std::string &fragment_src) {
        return std::make_shared<Shader_OpenGL>(name, vertex_src, fragment_src);
    }

    /* below for Shader_OpenGL */
    void Shader_OpenGL::setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void Shader_OpenGL::setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void Shader_OpenGL::setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }

    void Shader_OpenGL::setVec2(const std::string& name, float x, float y) const {
        glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
    }

    void Shader_OpenGL::setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }

    void Shader_OpenGL::setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
    }

    void Shader_OpenGL::setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }

    void Shader_OpenGL::setVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
    }

    void Shader_OpenGL::setMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader_OpenGL::setMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader_OpenGL::setMat4(const std::string& name, const glm::mat4& mat) const {
        GLCall(glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]););
    }

    void Shader_OpenGL::setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
    }

    Shader_OpenGL::Shader_OpenGL(const std::string &all_src) {
        ShaderProgramSource sps = ParseShader(all_src);

        uint32_t program_id = CreateShader(sps.VertexSource, sps.FragmentSource);
        m_id = program_id;

        auto lastSlash = all_src.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = all_src.rfind('.');
        auto count = lastDot == std::string::npos ? all_src.size() - lastSlash : lastDot - lastSlash;

        m_name = all_src.substr(lastSlash, count);
    }

    Shader_OpenGL::Shader_OpenGL(const std::string &name, const std::string &vertex_src, const std::string &fragment_src) {
        m_name = name;

        uint32_t program_id = CreateShader(vertex_src, fragment_src);
        m_id = program_id;
    }

    Shader_OpenGL::~Shader_OpenGL() {
        glDeleteProgram(m_id);
    }

    void Shader_OpenGL::bind() const {
        glUseProgram(m_id);
    }

    void Shader_OpenGL::unbind() const {
        glUseProgram(0);
    }

    /* below for Shader Stack */
    void ShaderStack::Add(const std::shared_ptr<Shader> &rhs) {
        auto& name = rhs->GetName();
        Add(name, rhs);
    }

    void ShaderStack::Add(const std::string &rhs_name, const std::shared_ptr<Shader> &rhs_shader) {
        if (Exists(rhs_name)){
            std::cout << "Warn: When Add " << rhs_name << ", detected this item already exits." << std::endl;
        }
        m_Shaders[rhs_name] = rhs_shader;
    }

    std::shared_ptr<Shader> ShaderStack::Load(const std::string &rhs_filepath) {
        auto ans_shader = Shader::create(rhs_filepath);
        Add(ans_shader);
        return ans_shader;
    }

    std::shared_ptr<Shader> ShaderStack::Load(const std::string &rhs_name, const std::string &rhs_filepath) {
        auto ans_shader = Shader::create(rhs_filepath);
        Add(rhs_name, ans_shader);
        return ans_shader;
    }

    std::shared_ptr<Shader> ShaderStack::Get(const std::string &rhs_name) {
        if (!Exists(rhs_name)){
            std::cout << "Err: When get " << rhs_name << " shader. Can't find it" << std::endl;
        }
        return m_Shaders[rhs_name];
    }

    bool ShaderStack::Exists(const std::string &rhs_name) const {
        return m_Shaders.find(rhs_name) != m_Shaders.end();
    }


}
