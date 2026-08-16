#pragma once

#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "Transform2D.h"
#include "OrthoCamera.h"
#include "gl/Buffers.h"

struct TRS_BatchAoSRenderer {
    Transform2D::AoS batch;
    gl::Buffer buffer;

    glm::vec2 cachedCameraPosition;
    glm::mat4 cachedCameraProjection;

    TRS_BatchAoSRenderer()
        : buffer(GL_ARRAY_BUFFER)
    {}

    void setCameraState(OrthoCamera &camera) {
        cachedCameraPosition = camera.position;
        cachedCameraProjection = camera.projection;
    }

    void initialize();
    void bindRenderMesh(RenderMesh &renderMesh);
    void submit(const Transform2D &transform);
    void submitBatch(Transform2D::Container &transforms);
    void render(RenderMesh &renderMesh, RenderMaterial &renderMat);
};

