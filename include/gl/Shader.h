#pragma once

#include <iostream>

#include <cstddef>
#include <glad/gl.h> 
#include <string_view>

namespace gl {
    struct Shader {
    private:
        int type;
        unsigned int id{};
        std::string_view sourceView;

    public:
        Shader(int type, std::string_view source);
        ~Shader();
        void initialize();

        void compile() const;
        void del() const;

        unsigned int getId() const { return id; }

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
