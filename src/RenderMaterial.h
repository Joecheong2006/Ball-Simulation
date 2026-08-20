#pragma once

#include "gl/ShaderProgram.h"

struct RenderMaterial {
    gl::ShaderProgram shaderProgram;
    gl::Shader vertex, fragment;

    RenderMaterial(gl::Shader vertex, gl::Shader fragment) noexcept;
    void initialize();
};

