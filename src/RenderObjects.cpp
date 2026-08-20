#include "RenderObjects.h"

#include "RenderMesh.h"
#include "RenderMaterial.h"

#include "profiling.h"

void RenderObjects::initialize() {
    ZoneScoped;
    for (auto i = 0; i < meshInitializers.size(); ++i) {
        auto &mesh = meshes.emplace_back(meshInitializers[i]());
        mesh.initialize();
    }

    for (auto i = 0; i < matInitializers.size(); ++i) {
        auto &mat = materials.emplace_back(matInitializers[i]());
        mat.initialize();
    }
}

int RenderObjects::addRenderMeshInitializer(MeshInitializer &&initializer) {
    ZoneScoped;
    meshInitializers.emplace_back(std::move(initializer));
    return static_cast<int>(meshInitializers.size()) - 1;
}

int RenderObjects::addRenderMaterialInitializer(MaterialInitializer &&initializer) {
    ZoneScoped;
    matInitializers.emplace_back(std::move(initializer));
    return static_cast<int>(matInitializers.size()) - 1;
}

RenderMesh &RenderObjects::getRenderMesh(int id) {
    ZoneScoped;
    return meshes[id];
}

RenderMaterial &RenderObjects::getRenderMaterial(int id) {
    ZoneScoped;
    return materials[id];
}
