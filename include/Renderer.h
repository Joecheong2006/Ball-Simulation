#pragma once

#include "RenderObjects.h"
#include "BatchRenderer.h"
#include <vector>

class Renderer {
    RenderObjects renderObjects;
    std::vector<BatchRenderer> renderers;

public:
    void initialize(RenderObjects &&renderObjects);
    void submit(int meshId, int matId, const Transform2D &transform);
    void submitBatch(int meshId, int matId, const Transform2D::Container &transforms);
    void render(OrthoCamera &camera);

};

