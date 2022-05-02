/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/include/renderer_model.hpp
 * @brief   A API package for model class.
 */

#ifndef H_RENDER_MODEL
#define H_RENDER_MODEL

#ifdef  WIN32
#pragma once
#endif //! WIN32

#include "render_base.hpp"
#include "render_buffer.hpp"

#define ASSIMP_LOAD_ENABLE  false
#define OBJLOADER_ENABLE    true

#if ASSIMP_LOAD_ENABLE == true
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#elif OBJLOADER_ENABLE == true
#include <tinyobjloader/tiny_obj_loader.h>
#endif

namespace evergarden {

    enum class MeshType {
        None    = 0,
        Model   = 1,
        Cube    = 2,
        Plane   = 3,
        Sphere  = 4
    };

    class Mesh {
    public:
        Mesh():c_Type(MeshType::None), c_Name("mesh"), c_Path(" ") {};
        explicit Mesh(const std::string& path);
        explicit Mesh(const MeshType& type);
        Mesh(const MeshType& type, int sample);
        Mesh(std::vector<Vertex>  vertices, std::vector<uint32_t>  indices);

        ~Mesh() = default;

    public:
        void Draw(bool lineMode = false);

    public:
        static std::shared_ptr<Mesh> Create() { return std::make_shared<Mesh>(); }
        static std::shared_ptr<Mesh> Create(const std::string& path) { return std::make_shared<Mesh>(path); }
        static std::shared_ptr<Mesh> Create(const MeshType& type) {
            return std::make_shared<Mesh>(type);
        }
        static std::shared_ptr<Mesh> Create(const MeshType& type, int sample) {
            return std::make_shared<Mesh>(type, sample);
        }
        static std::shared_ptr<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
            return std::make_shared<Mesh>(vertices, indices);
        }

    public:
        //	Get attribute
        size_t GetVerticesNum() const   { return c_Vertices.size(); }
        size_t GetTriangleNum() const   { return c_Triangles.size(); }
        MeshType GetMeshType() const    { return c_Type; }
        std::string GetPathName() const { return c_Path; }
        bool HasNormal() const          { return c_HasNormal; }
        bool HasTexCoord() const        { return c_HasTexCoord; }

    private:
        void CreatePlane();
        void CreateCube();
        void CreateSphere();

    public:
        void AssimpFileLoader(const std::string &FilePath);
        void ObjFileLoader(const std::string &FilePath);

    private:
        void GenVertexArray();
        void GenTBN();

    private:
        // Class based parameters
        std::string c_Name {};
        std::string c_Path {};
        MeshType    c_Type;

        //	buffer
        std::shared_ptr<VertexArray> c_VertexArray;

        // Basic data
        std::vector<Vertex>     c_Vertices;
        std::vector<uint32_t>   c_Indices;
        std::vector<Triangle>   c_Triangles;

        // Numerous flags.
        bool c_HasNormal = false;
        bool c_HasTexCoord = false;
    };

} //! namespace evergarden

#endif //! H_RENDER_MODEL
