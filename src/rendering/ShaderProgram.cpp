
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) : id(0) {
    Construct(vertexShader, fragmentShader);
}

void ShaderProgram::Construct (const Shader& vertexShader, const Shader& fragmentShader) {
    id = glCreateProgram();
    glAttachShader(id, vertexShader.id);
    glAttachShader(id, fragmentShader.id);
    glLinkProgram(id);
    // Check for errors
    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to link shader program: " + std::string(infoLog));
    }
}

bool ShaderProgram::IsValid() const {
    return id != 0;
}

ShaderProgram::~ShaderProgram() {
    if (IsValid()) {
        glDeleteProgram(id);
    }
}

void ShaderProgram::Bind() const { glUseProgram(id); }

void ShaderProgram::UnbindAny() { glUseProgram(0); }
void ShaderProgram::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}
void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

