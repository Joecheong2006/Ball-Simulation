#pragma once
#include <functional>

#include "RenderMaterial.h"

struct RenderMesh;
class RenderObjects {
    using MeshInitializer = std::function<RenderMesh()>;
    std::vector<MeshInitializer> meshInitializers;
    std::vector<RenderMesh> meshes;

    using MaterialInitializer = std::function<RenderMaterial()>;
    std::vector<MaterialInitializer> matInitializers;
    std::vector<RenderMaterial> materials;

public:
    void initialize();
    int addRenderMeshInitializer(MeshInitializer &&initializer);
    int addRenderMaterialInitializer(MaterialInitializer &&initializer);
    RenderMesh &getRenderMesh(int id);
    RenderMaterial &getRenderMaterial(int id);

    int getRenderMeshSize() const { return static_cast<int>(meshes.size()); }
    int getRenderMaterialSize() const { return static_cast<int>(materials.size()); }

};
