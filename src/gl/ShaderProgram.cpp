#include "gl/ShaderProgram.h"
#include "profiling.h"

namespace gl {
    void ShaderProgram::initialize() {
        ZoneScoped;
        id = glCreateProgram();
    }

    ShaderProgram::~ShaderProgram() {
        if (id)
            glDeleteProgram(id);
    }

    void ShaderProgram::attachShader(const Shader &shader) const {
        ZoneScoped;
        glAttachShader(id, shader.getId());
    }

    void ShaderProgram::link() const {
        ZoneScoped;
        glLinkProgram(id);
    }

    void ShaderProgram::use() const {
        ZoneScoped;
        glUseProgram(id);
    }

    void ShaderProgram::setUniform1i(const std::string &name, int val) {
        ZoneScoped;
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform1i(id, val);
        }
    }

    void ShaderProgram::setUniform1f(const std::string &name, float val) {
        ZoneScoped;
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform1f(id, val);
        }
    }

    void ShaderProgram::setUniform2f(const std::string &name, float x, float y) {
        ZoneScoped;
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform2f(id, x, y);
        }
    }

    void ShaderProgram::setUniform2f(const std::string &name, const glm::vec2 &v) {
        ZoneScoped;
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniform2f(id, v.x, v.y);
        }
    }

    void ShaderProgram::setUniformMat4(const std::string &name, const glm::mat4 &m) {
        ZoneScoped;
        int id = getUniformLocation(name);
        if (id != -1) {
            glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(m));
        }
    }

    int ShaderProgram::getUniformLocation(const std::string &name) {
        ZoneScoped;
        if(m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
            return m_uniform_location_cache[name];
        int location = glGetUniformLocation(id, name.c_str());
        if (location != -1) {
            m_uniform_location_cache[name] = location;
        }
        return location;
    }

}
