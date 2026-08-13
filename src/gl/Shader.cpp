#include "gl/Shader.h"

namespace gl {
    Shader::Shader(int type, const char *source)
        : id(glCreateShader(type))
    {
        glShaderSource(id, 1, &source, NULL);
    }

    Shader::~Shader() {
        del();
    }

    void Shader::compile() const {
        glCompileShader(id);
        LogErrors(*this);
    }

    void Shader::del() const{
        glDeleteShader(id);
    }

}
