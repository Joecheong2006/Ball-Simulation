#include "RenderMaterial.h"

#include "profiling.h"

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
    shaderProgram.link();
}
