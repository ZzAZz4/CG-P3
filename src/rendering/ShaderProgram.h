

#ifndef OGL_SHADERPROGRAM_H
#define OGL_SHADERPROGRAM_H

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

struct ShaderProgram {
    int id = 0;

    ShaderProgram() = default;
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&&) noexcept = delete;
    ShaderProgram& operator=(ShaderProgram&) = delete;
    ShaderProgram& operator=(ShaderProgram&&) noexcept = delete;

    bool IsValid() const;
    void Construct(const Shader& vertexShader, const Shader& fragmentShader);

    void Bind() const;
    static void UnbindAny();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif // OGL_SHADERPROGRAM_H
