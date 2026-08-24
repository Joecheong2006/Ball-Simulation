#pragma once

#include "RenderMesh.hpp"
#include "Transform2D.hpp"
#include "gl/Buffer.hpp"

#include <unordered_map>

struct OrthoCamera;
class RenderObjects;

struct TRS_BatchAoSRenderer {
    std::unordered_map<int, Transform2D::AoS> batches;
    gl::Buffer buffer{GL_ARRAY_BUFFER};
    void (*renderCall)(const RenderMesh &renderMesh, int size);

    TRS_BatchAoSRenderer() = default;

    void initialize(const RenderMesh &renderMesh);
    void bindLayout(const gl::BufferLayout &layout) const;
    void submit(int matId, const Transform2D &transform);
    void submitBatch(int matId, const Transform2D::Container &transforms);
    void submitBatch(int matId, const Transform2D::Container &transforms, int size);
    void render(int meshId, OrthoCamera &camera, RenderObjects &renderObjects);
};

