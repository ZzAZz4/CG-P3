#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"

SimpleMesh load_simple_obj (std::string_view inputfile) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "../res/animation/"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(std::string(inputfile), reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }


    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    SimpleMesh mesh;

    for (const auto& shape: shapes) {
        size_t index_offset = 0;
        size_t face_verts = shape.mesh.num_face_vertices.size();

        for (size_t f = 0; f < face_verts; f++) {
            auto fv = size_t(shape.mesh.num_face_vertices[f]);
            for (size_t v = 0; v < fv; v++) {
                auto i = shape.mesh.indices[index_offset + v].vertex_index;
                mesh.vertices.emplace_back( attrib.vertices[3 * i], attrib.vertices[3 * i + 1], attrib.vertices[3 * i + 2] );
            }
            index_offset += fv;
        }
    }
    mesh.normals.resize(mesh.vertices.size());
    for (size_t i = 0; i < mesh.vertices.size() / 3; i += 3) {
        auto v1 = mesh.vertices[i];
        auto v2 = mesh.vertices[i + 1];
        auto v3 = mesh.vertices[i + 2];
        auto normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        mesh.normals[i] = normal;
        mesh.normals[i + 1] = normal;
        mesh.normals[i + 2] = normal;
    }
    return mesh;

}

void SimpleMesh::Start () {
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, &vbo[0]);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, (long long) (vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, (long long) (normals.size() * sizeof(glm::vec3)), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    glPointSize(2.0f);
}

void SimpleMesh::Update () {

}

SimpleMesh::~SimpleMesh () {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, vbo);
}

void SimpleMesh::Render (const ShaderProgram& shaderProgram) {
    shaderProgram.setMat4("model", glm::mat4(1.0f));

    assert(vao != 0);
    glBindVertexArray(vao);
    if (renderMode == RenderMode::TRIANGLES) {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    } else if (renderMode == RenderMode::TRIANGLE_LINES) {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    } else if (renderMode == RenderMode::POINTS) {
        glDrawArrays(GL_POINTS, 0, (GLsizei) vertices.size());
    }
    glBindVertexArray(0);
}

