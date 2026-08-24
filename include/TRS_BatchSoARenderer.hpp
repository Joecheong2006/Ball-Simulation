#pragma once

#include "RenderMesh.hpp"
#include "Transform2D.hpp"
#include "gl/Buffers.hpp"

#include <unordered_map>

struct OrthoCamrea;
class RenderObjects;

struct TRS_BatchSoARenderer {
    std::unordered_map<int, Transform2D::SoA> batches;
    gl::Buffers buffers{GL_ARRAY_BUFFER};
    void (*renderCall)(const RenderMesh &renderMesh, int size);

    TRS_BatchSoARenderer() = default;

    void initialize(const RenderMesh &renderMesh);
    void bindLayout(const gl::BufferLayout &layout) const;
    void submit(int matId, const Transform2D &transform);
    void submitBatch(int matId, const Transform2D::Container &transforms);
    void submitBatch(int matId, const Transform2D::Container &transforms, int size);
    void render(int meshId, OrthoCamera &camera, RenderObjects &renderObjects);
};

