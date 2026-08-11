#include "ShaderProgram.h"

namespace gl {
    ShaderProgram::ShaderProgram() : id(glCreateProgram()) {}

    void ShaderProgram::attachShader(const Shader &shader) const {
        glAttachShader(id, shader.id);
    }

    void ShaderProgram::link() const {
        glLinkProgram(id);
    }

    void ShaderProgram::use() const {
        glUseProgram(id);
    }

    void ShaderProgram::setUniform1i(const std::string &name, int val) {
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform1i(id, val);
        }
    }

    void ShaderProgram::setUniform1f(const std::string &name, float val) {
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform1f(id, val);
        }
    }

    void ShaderProgram::setUniform2f(const std::string &name, float x, float y) {
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform2f(id, x, y);
        }
    }

    void ShaderProgram::setUniform2f(const std::string &name, const glm::vec2 &v) {
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform2f(id, v.x, v.y);
        }
    }

    void ShaderProgram::setUniformMat4(const std::string &name, const glm::mat4 &m) {
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(m));
        }
    }

    int ShaderProgram::getUniformLocation(const std::string &name) {
        if(m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
            return m_uniform_location_cache[name];
        int location = glGetUniformLocation(id, name.c_str());
        if (location != -1) {
            m_uniform_location_cache[name] = location;
        }
        return location;
    }

}
