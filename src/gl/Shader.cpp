#include "gl/Shader.h"
#include "profiling.h"

namespace gl {
    Shader::Shader(int type, const char *source)
        : id(glCreateShader(type))
    {
        ZoneScoped;
        glShaderSource(id, 1, &source, NULL);
    }

    Shader::~Shader() {
        ZoneScoped;
        del();
    }

    void Shader::compile() const {
        ZoneScoped;
        glCompileShader(id);
        LogErrors(*this);
    }

    void Shader::del() const{
        ZoneScoped;
        glDeleteShader(id);
    }

}
