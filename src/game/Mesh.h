

#ifndef HOMEWORK_MESH_H
#define HOMEWORK_MESH_H

#include <tiny_obj_loader.h>
#include <iostream>
#include <glm/glm.hpp>
#include <rendering/ShaderProgram.h>

enum class RenderMode {
    POINTS,
    TRIANGLE_LINES,
    TRIANGLES,
};

struct SimpleMesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    unsigned int vao = 0, vbo[2] = {0, 0};
    RenderMode renderMode = RenderMode::TRIANGLE_LINES;

    SimpleMesh() = default;
    ~SimpleMesh();

    void Start();
    void Update();
    void Render(const ShaderProgram& shaderProgram);
};

SimpleMesh load_simple_obj(std::string_view inputfile);


#endif //HOMEWORK_MESH_H
