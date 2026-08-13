#pragma once

#include <iostream>

#include <cstddef>
#include <glad/gl.h> 

namespace gl {
    struct Shader {
        unsigned int id;

        Shader(int type, const char *source);
        ~Shader();

        void compile() const;

        void del() const;

        inline static void LogErrors(const Shader &shader) {
            int  success;
            char infoLog[512];
            glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(shader.id, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }

    };

}
