#pragma once

#include "RenderObjects.hpp"
#include "BatchRenderer.hpp"
#include <vector>

class Renderer {
    RenderObjects renderObjects;
    std::vector<BatchRenderer> renderers;

public:
    void initialize(RenderObjects &&renderObjects);
    void submit(int meshId, int matId, const Transform2D &transform);
    void submitBatch(int meshId, int matId, const Transform2D::Container &transforms);
    void submitBatch(int meshId, int matId, const Transform2D::Container &transforms, int size);
    void render(OrthoCamera &camera);

};

