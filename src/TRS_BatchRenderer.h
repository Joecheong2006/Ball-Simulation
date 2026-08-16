#pragma once
#include <glm/glm.hpp>

#include "TaggedPointer.h"
#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "Transform2D.h"
#include "OrthoCamera.h"

struct TRS_BatchSoARenderer;
struct TRS_BatchAoSRenderer;

struct TRS_BatchRenderer : TaggedPointer<TRS_BatchSoARenderer, TRS_BatchAoSRenderer> {
    using TaggedPointer::TaggedPointer;

    void initialize();
    void bindRenderMesh(RenderMesh &renderMesh);
    void submit(const Transform2D &transform);
    void submitBatch(Transform2D::Container &transforms);
    void render(RenderMesh &renderMesh, RenderMaterial &renderMat);
    void setCameraState(OrthoCamera &camera);

};

