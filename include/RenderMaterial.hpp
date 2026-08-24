#pragma once

#include "gl/ShaderProgram.hpp"

struct OrthoCamera;
class RenderMaterial {
    gl::ShaderProgram shaderProgram;
    gl::Shader vertex, fragment;

public:
    RenderMaterial(gl::Shader vertex, gl::Shader fragment) noexcept;
    void initialize();

    void activate() const;
    void setShaderCameraState(const OrthoCamera &camera);

};

