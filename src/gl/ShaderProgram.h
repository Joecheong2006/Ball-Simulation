#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <unordered_map>

#include <cstddef>
#include <glad/gl.h> 

namespace gl {
    struct ShaderProgram {
        ShaderProgram();

        void attachShader(const Shader &shader) const;
        void link() const;
        void use() const;
        void setUniform1i(const std::string &name, int val);
        void setUniform1f(const std::string &name, float val);
        void setUniform2f(const std::string &name, float x, float y);
        void setUniform2f(const std::string &name, const glm::vec2 &v);
        void setUniformMat4(const std::string &name, const glm::mat4 &m);
        int getUniformLocation(const std::string &name);

    private:
        unsigned int id;
        std::unordered_map<std::string, int> m_uniform_location_cache;

    };

}
