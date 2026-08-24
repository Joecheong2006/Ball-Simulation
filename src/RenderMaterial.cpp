#include "RenderMaterial.hpp"

#include "OrthoCamera.hpp"
#include "profiling.hpp"

RenderMaterial::RenderMaterial(gl::Shader vertex, gl::Shader fragment) noexcept
    : vertex(vertex), fragment(fragment)
{
    ZoneScoped;
}

void RenderMaterial::initialize() {
    ZoneScoped;
    shaderProgram.initialize();

    vertex.initialize();
    vertex.compile();

    fragment.initialize();
    fragment.compile();

    shaderProgram.attachShader(vertex);
    shaderProgram.attachShader(fragment);

    vertex.del();
    fragment.del();

    shaderProgram.link();
}

void RenderMaterial::activate() const {
    ZoneScoped;
    shaderProgram.use();
}

void RenderMaterial::setShaderCameraState(const OrthoCamera &camera) {
    ZoneScoped;
    shaderProgram.setUniform2f("camPos", camera.position);
    shaderProgram.setUniformMat4("projection", camera.projection);
}
