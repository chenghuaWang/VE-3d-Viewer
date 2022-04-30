/*!
 * @author  chenghua.wang
 * @file    Evergarden/Rednerer/scr/renderer_model.cpp
 * @brief   A API package for model class.
 */

#include "../include/render_model.hpp"

namespace evergarden {

    Mesh::Mesh(const std::string &path) {
        c_Type = MeshType::Model;
#if ASSIMP_LOAD_ENABLE == true
        AssimpFileLoader(path);
#elif OBJLOADER_ENABLE == true
        ObjFileLoader(path);
#endif
        GenTBN();
        GenVertexArray();
    }

    Mesh::Mesh(const MeshType &type):c_Type(type) {
        c_HasNormal = true;
        c_HasTexCoord = true;
        switch (type) {
            case MeshType::None:
                return;
            case MeshType::Plane:
                CreatePlane();
                break;
            case MeshType::Cube:
                CreateCube();
                break;
            case MeshType::Sphere:
                CreateSphere();
                break;
            case MeshType::Model:
                return;
            default:
                break;
        }
        GenTBN();
        GenVertexArray();
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices):
            c_Type(MeshType::Model), c_Vertices(std::move(vertices)), c_Indices(std::move(indices)) {

        size_t len_indices = c_Indices.size();
        for (size_t i = 0; i < len_indices; i += 3) {
            Triangle triangle = { c_Indices[i],c_Indices[i + 1ul],c_Indices[i + 2ul] };
            c_Triangles.push_back(triangle);
        }
        GenTBN();
        GenVertexArray();
    }

    void Mesh::Draw(bool lineMode) {
        if (c_Type == MeshType::None) return;
        if (lineMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        c_VertexArray->Bind();
        glDrawElements(GL_TRIANGLES, c_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Mesh::CreatePlane() {
        // TODO from Asset import a plane.obj file
        ObjFileLoader(" ");
    }

    void Mesh::CreateSphere() {
        // TODO from Asset import a sphere.obj file
        ObjFileLoader(" ");
    }

    void Mesh::CreateCube() {
        // TODO from Asset import a Cube.obj file
        ObjFileLoader(" ");
    }

    void Mesh::AssimpFileLoader(const std::string &FilePath) {
        return;
    }

    void Mesh::ObjFileLoader(const std::string &FilePath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath.c_str())) {
            c_Type = MeshType::None;
            return;
        }
        c_Path = static_cast<std::string>(FilePath);

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        if (!attrib.normals.empty()) {
            c_HasNormal = true;
        } else {
            c_HasNormal = false;
        }

        if (!attrib.texcoords.empty()) {
            c_HasTexCoord = true;
        } else {
            c_HasTexCoord = false;
        }
        for (auto const& shape : shapes) {
            for (auto const &index: shape.mesh.indices) {
                Vertex v{};
                if (attrib.vertices.size() > 3 * index.vertex_index + 2) {
                    v.Position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]
                    };
                }
                if (!attrib.texcoords.empty()) {
                    v.TexCoords = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }
                else v.TexCoords = {0.0f, 0.0f};
                if (!attrib.normals.empty()) {
                    v.Normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2]
                    };
                } else {
                    v.Normal = {0.0f, 0.0f, 0.0f};
                }
                if (uniqueVertices.count(v) == 0) {
                    uniqueVertices[v] = static_cast<uint32_t>(c_Vertices.size());
                    c_Vertices.push_back(v);
                }
                c_Indices.push_back(uniqueVertices[v]);
            }
        }

        size_t len_indices = c_Indices.size();
        for (size_t i = 0; i < len_indices; i += 3) {
            Triangle triangle = { c_Indices[i],c_Indices[i + 1ul],c_Indices[i + 2ul] };
            c_Triangles.push_back(triangle);
        }
    }

    void Mesh::GenVertexArray() {
        c_VertexArray = VertexArray::Create();

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer = VertexBuffer::Create((float*)(&*c_Vertices.begin()), sizeof(Vertex) * c_Vertices.size());

        BufferLayout layout = {
                {InnerDataType::Vec3f, "a_Position"},
                {InnerDataType::Vec2f, "a_TexCoord"},
                {InnerDataType::Vec3f, "a_Normal"},
                {InnerDataType::Vec3f, "a_Tangent"},
                {InnerDataType::Vec3f, "a_Bitangent"}
        };

        vertexBuffer->SetLayout(layout);
        c_VertexArray->AddVertexBuffer(vertexBuffer);

        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer = IndexBuffer::Create((uint32_t*)(&*c_Indices.begin()), c_Indices.size());
        c_VertexArray->AddIndexBuffer(indexBuffer);
    }

    void Mesh::GenTBN() {
        if (!c_HasNormal) {
            for (auto triangle : c_Triangles) {
                glm::vec3 v1 = c_Vertices[triangle.id1].Position - c_Vertices[triangle.id2].Position;
                glm::vec3 v2 = c_Vertices[triangle.id2].Position - c_Vertices[triangle.id3].Position;

                glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

                c_Vertices[triangle.id1].Normal = glm::normalize(c_Vertices[triangle.id1].Normal + normal);
                c_Vertices[triangle.id2].Normal = glm::normalize(c_Vertices[triangle.id2].Normal + normal);
                c_Vertices[triangle.id3].Normal = glm::normalize(c_Vertices[triangle.id3].Normal + normal);
            }
        }

        for (auto& triangle : c_Triangles) {
            glm::vec3 norm = (c_Vertices[triangle.id1].Normal + c_Vertices[triangle.id2].Normal + c_Vertices[triangle.id3].Normal) * glm::vec3(1.0f / 3.0f);

            glm::vec3 edge1 = c_Vertices[triangle.id2].Position - c_Vertices[triangle.id1].Position;
            glm::vec3 edge2 = c_Vertices[triangle.id1].Position - c_Vertices[triangle.id3].Position;
            glm::vec2 deltaUV1 = c_Vertices[triangle.id2].TexCoords - c_Vertices[triangle.id1].TexCoords;
            glm::vec2 deltaUV2 = c_Vertices[triangle.id1].TexCoords - c_Vertices[triangle.id3].TexCoords;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent;
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            glm::vec3 bitangent;
            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);

            c_Vertices[triangle.id1].Tangent = glm::normalize(c_Vertices[triangle.id1].Tangent + tangent);
            c_Vertices[triangle.id2].Tangent = glm::normalize(c_Vertices[triangle.id2].Tangent + tangent);
            c_Vertices[triangle.id3].Tangent = glm::normalize(c_Vertices[triangle.id3].Tangent + tangent);

            c_Vertices[triangle.id1].Bitangent = glm::normalize(c_Vertices[triangle.id1].Bitangent + bitangent);
            c_Vertices[triangle.id2].Bitangent = glm::normalize(c_Vertices[triangle.id2].Bitangent + bitangent);
            c_Vertices[triangle.id3].Bitangent = glm::normalize(c_Vertices[triangle.id3].Bitangent + bitangent);
        }
    }

}
