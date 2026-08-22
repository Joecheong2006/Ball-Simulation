#include "RenderObjects.h"

#include "RenderMesh.h"
#include "RenderMaterial.h"

#include "profiling.h"

void RenderObjects::initialize() {
    ZoneScoped;
    meshes.reserve(meshInitializers.size());
    for (auto i = 0; i < meshInitializers.size(); ++i) {
        meshes.push_back(meshInitializers[i]());
        meshes[i].initialize();
    }

    materials.reserve(matInitializers.size());
    for (auto i = 0; i < matInitializers.size(); ++i) {
        materials.push_back(matInitializers[i]());
        materials[i].initialize();
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
