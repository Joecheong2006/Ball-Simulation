#include "RenderMaterial.h"

#include "profiling.h"

RenderMaterial::RenderMaterial(const gl::Shader &vertex, const gl::Shader &fragment) {
    ZoneScoped;
    shaderProgram.initialize();
    vertex.compile();
    fragment.compile();

    shaderProgram.attachShader(vertex);
    shaderProgram.attachShader(fragment);
    shaderProgram.link();
}
