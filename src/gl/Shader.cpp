#include "gl/Shader.h"
#include "profiling.h"

namespace gl {
    Shader::Shader(int type, std::string_view source)
        : type(type), sourceView(source)
    {
        ZoneScoped;
    }

    Shader::~Shader() {
        ZoneScoped;
        del();
    }

    void Shader::initialize() {
        ZoneScoped;
        id = glCreateShader(type);
        const char *src_ptr = sourceView.data();
        glShaderSource(id, 1, &src_ptr, NULL);
    }

    void Shader::compile() const {
        ZoneScoped;
        glCompileShader(id);
        LogErrors(*this);
    }

    void Shader::del() const{
        ZoneScoped;
        if (id)
            glDeleteShader(id);
    }

}
