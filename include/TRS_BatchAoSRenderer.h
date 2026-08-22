#pragma once

#include "RenderMesh.h"
#include "Transform2D.h"
#include "gl/Buffer.h"

#include <unordered_map>

struct OrthoCamera;
class RenderObjects;

struct TRS_BatchAoSRenderer {
    std::unordered_map<int, Transform2D::AoS> batches;
    gl::Buffer buffer{GL_ARRAY_BUFFER};
    void (*renderCall)(RenderMesh &renderMesh, int size);

    TRS_BatchAoSRenderer() = default;

    void initialize(const RenderMesh &renderMesh);
    void submit(int matId, const Transform2D &transform);
    void submitBatch(int matId, const Transform2D::Container &transforms);
    void submitBatch(int matId, const Transform2D::Container &transforms, int size);
    void render(OrthoCamera &camera, RenderMesh &renderMesh, RenderObjects &renderObjects);
};

