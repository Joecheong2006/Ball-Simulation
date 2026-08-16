#pragma once

#include "gl/ShaderProgram.h"

struct RenderMaterial {
    gl::ShaderProgram shaderProgram;

    RenderMaterial(const gl::Shader &vertex, const gl::Shader &fragment);
};

