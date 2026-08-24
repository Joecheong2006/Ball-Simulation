#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image/stb_image.h"

#include <vector>
#include <assert.h>
#include <random>

constexpr float pi = 3.141592f;

// 
// TODO: make vertex shader generic
//
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 10) in vec2 aInstancePos;
layout (location = 11) in vec2 aInstanceScale;
layout (location = 12) in float aInstanceAngle;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform vec2 camPos;

void main() {
    TexCoord = aTexCoord;

    float c = cos(aInstanceAngle);
    float s = sin(aInstanceAngle);

    mat4 model = mat4(
        c * aInstanceScale.x, s * aInstanceScale.x, 0.0, 0.0,
        -s * aInstanceScale.y, c * aInstanceScale.y, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        aInstancePos.x, aInstancePos.y, 0.0, 1.0
    );

    mat4 view = mat4(
        1.0,  0.0,  0.0,  0.0,
        0.0,  1.0,  0.0,  0.0,
        0.0,  0.0,  1.0,  0.0,
        -camPos.x, -camPos.y, 0.0, 1.0
    );

    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main() {
    vec2 uv = TexCoord * 2.0f - 1.0f;
    if (uv.x * uv.x + uv.y * uv.y < 1.0) {
        FragColor = vec4(1);
    }
    else {
        FragColor = vec4(0);
    }
}
)";

const char *fragmentShaderSource2 = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main() {
    FragColor = vec4(0.2, 0.5, 0.8, 1);
}
)";

#include "gl/BufferLayout.hpp"

#include "MainWindow.hpp"

class BumpAllocator {
    std::byte *bytes;
    int top{};

public:
    BumpAllocator(int size)
        : bytes(new std::byte[size])
    {}

    ~BumpAllocator() {
        delete[] bytes;
    }

    BumpAllocator(BumpAllocator &&) = delete;
    void operator=(BumpAllocator &&) = delete;
    
    template <typename T, typename... Args>
    T *alloc(Args&&... args) {
        auto location = top;

        T *ptr = new(bytes + location) T(std::forward<Args>(args)...);

        top += sizeof(T);
        return ptr;
    }

    void clear() {
        top = 0;
    }

};

#include "TaggedPointer.hpp"
#include "Transform2D.hpp"

struct AABB {
    glm::vec2 min, max;

    bool intersect(const AABB &aabb) const {
        return (min.x <= aabb.max.x && max.x >= aabb.min.x) &&
               (min.y <= aabb.max.y && max.y >= aabb.min.y);
    }
};

struct Physics2DState {
    glm::vec2 p, v, a{ 0, -9.81f };
    float theta;
};

struct Circle {
    float radius;
    AABB getAABB(const Physics2DState &state) const {
        return {
            state.p - glm::vec2(radius), state.p + glm::vec2(radius)
        };
    }
};

struct Box {
    glm::vec2 size;
    AABB getAABB(const Physics2DState &state) const {
        glm::vec2 r = glm::vec2{ size.x * cos(state.theta), size.y * sin(state.theta) } * 0.5f;
        return {
            state.p - r, state.p + r
        };
    }
};

struct Shape : public TaggedPointer<Circle, Box> {
    using TaggedPointer::TaggedPointer;

    AABB getAABB(const Physics2DState &state) const {
        return Dispatch([&state](auto *obj) {
                return obj->getAABB(state);
            });
    }

};

struct Physics2DStates {
    std::vector<glm::vec2> p, v, a;
    std::vector<float> theta;

    Physics2DState at(int i) const {
        return {
            p[i], v[i], a[i], theta[i]
        };
    }
};

struct Physics2DWorld {
    std::vector<Shape> shapes;
    std::vector<AABB> AABBs;
    Physics2DStates states;

    template <typename T>
    void add(T *obj, const Physics2DState &state = {}) {
        shapes.emplace_back(obj);
        addPhysicsState(state);
        AABBs.push_back(obj->getAABB(state));
    }

    void addPhysicsState(const Physics2DState &state) {
        states.p.push_back(state.p);
        states.v.push_back(state.v);
        states.a.push_back(state.a);
        states.theta.push_back(state.theta);
    }

    void updatePhysicsStates(Transform2D::SoA &transforms, float dt) {
        ZoneScoped;
        float dt2 = dt * dt;

        // int range = static_cast<int>(shapes.size());
        // std::vector<std::pair<int, int>> splits = {
        //     { 0, range / 4},
        //     { range / 4, range / 2 },
        //     { range / 2, range * 3 / 4 },
        //     { range * 3 / 4, range }
        // };
        //
        // for_each(std::execution::par, splits.begin(), splits.end(),
        //     [&](auto &range) {
        //         for (auto i = range.first; i < range.second; ++i) {
        //             states.v[i] += states.a[i] * dt;
        //             states.p[i] += 0.5f * states.a[i] * dt2 + states.v[i] * dt;
        //         }
        //     });

        for (auto i = 0; i < shapes.size(); ++i) {
            states.v[i] += states.a[i] * dt;
            states.p[i] += 0.5f * states.a[i] * dt2 + states.v[i] * dt;
        }
    }

    void updateAABBs() {
        ZoneScoped;

        for (auto i = 0; i < shapes.size(); ++i) {
            AABBs[i] = shapes[i].getAABB(states.at(i));
        }
    }

    void testCollisionAABBs() {
        ZoneScoped;
        static std::vector<glm::ivec2> collisions;
        collisions.clear();
        collisions.resize(shapes.size());

        for (auto i = 0; i < shapes.size(); ++i) {
            const auto &aabb = AABBs[i];
            for (auto j = i + 1; j < shapes.size(); ++j) {
                if (aabb.intersect(AABBs[j])) {
                    collisions.push_back({i, j});
                }
            }
        }
    }

    void updatePhysics(Transform2D::SoA &transforms, float dt) {
        ZoneScoped;
        updatePhysicsStates(transforms, dt);
        updateAABBs();
        testCollisionAABBs();
    }
};

struct World {
};

#include "RenderMesh.hpp"
#include "RenderMaterial.hpp"

#include "OrthoCamera.hpp"

struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
};

#include "RenderObjects.hpp"
#include "Renderer.hpp"

using FunctorGetRandom = glm::vec2(*)();
void randomCircles(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint, Transform2D::SoA &transforms);
void randomSquares(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint, Transform2D::SoA &transforms);

glm::vec2 randomPointFromHeart();

int main(void) {
    if (!glfwInit()) return -1;

    // Configure for Modern OpenGL Core Profile on macOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#endif

    // 
    // Window iniitalization
    //

    MainWindow::Initialize(1024, 720, "GLAD 2 Window");

    int width = MainWindow::Width(), height = MainWindow::Height();
    std::printf("width = %d, height = %d\n", width, height);

    // 
    // Enable Transparent
    //

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //
    // Initialzie RenderObjects
    //

    RenderObjects renderObjects;

    int quatId = renderObjects.addRenderMeshInitializer([]() {
            return RenderMesh({
                {   // Positions  // Texture coords
                     0.5f,  0.5f,  1.0f, 1.0f,
                     0.5f, -0.5f,  1.0f, 0.0f,
                    -0.5f, -0.5f,  0.0f, 0.0f,
                    -0.5f,  0.5f,  0.0f, 1.0f,
                },
                {
                    0, 1, 3,   // first triangle
                    1, 2, 3    // second triangle
                },
                gl::BufferLayout::Aggregate<Vertex>().getAttributes()
            });
        });

    int circleMaterialId = renderObjects.addRenderMaterialInitializer([]() {
            return RenderMaterial(
                { GL_VERTEX_SHADER, vertexShaderSource },
                { GL_FRAGMENT_SHADER, fragmentShaderSource }
            );
        });

    int blueMaterialId = renderObjects.addRenderMaterialInitializer([]() {
            return RenderMaterial(
                { GL_VERTEX_SHADER, vertexShaderSource },
                { GL_FRAGMENT_SHADER, fragmentShaderSource2 }
            );
        });

    std::pair<int, int> shapeRenderKeys[Shape::TotalTypes::value];
    shapeRenderKeys[Shape::TypeId<Circle>::value] = { quatId, circleMaterialId };
    shapeRenderKeys[Shape::TypeId<Box>::value] = { quatId, blueMaterialId };

    renderObjects.initialize();

    //
    // Initialzie Renderer
    //

    Renderer renderer;
    renderer.initialize(std::move(renderObjects));

    // 
    // 2D camrea creation
    //

    OrthoCamera camera({ 0, 0 }, { 1.0f, 1.0f }, -1.0f, 1.0f);

    // 
    // Setup physics world demo
    //

    Physics2DWorld physicsWorld{};

    // 
    // Initialize instnaces' circle
    //

    Transform2D::SoA transforms;

    { ZoneScopedN("Generating Shapes");
        int circleCount = 1000;
        randomCircles(circleCount, physicsWorld, randomPointFromHeart, transforms);

        int squareCount = 1000;
        randomSquares(squareCount, physicsWorld, randomPointFromHeart, transforms);
    }

    while (!MainWindow::ShouldClose()) {
        FrameMark;

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        physicsWorld.updatePhysics(transforms, 1.f / 1200);

        { ZoneScopedN("Submit");
            transforms.positions.assign(physicsWorld.states.p.begin(), physicsWorld.states.p.end());
            transforms.angles.assign(physicsWorld.states.theta.begin(), physicsWorld.states.theta.end());
            for (int i = 0; i < (int)physicsWorld.shapes.size(); ++i) {
                auto &shape = physicsWorld.shapes[i];
                auto &[meshId, matId] = shapeRenderKeys[shape.id()];
                renderer.submit(meshId, matId, transforms.at(i));
            }
        }

        FrameMarkStart("Render");
        renderer.render(camera);
        FrameMarkEnd("Render");

        MainWindow::SwapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

static std::mt19937 gen(std::random_device{}());
static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

glm::vec2 randomPointFromHeart() {
    float u1 = dis(gen) * 2.0f * pi;

    float x = 16.f * std::pow(sin(u1), 3.f);
    float y = 13.f * cos(u1) - 5.f * cos(2.f * u1) - 2.f * cos(3.f * u1) - cos(4.f * u1);

    float scale = 0.06f;

    x *= scale;
    y *= scale;

    float r = dis(gen);

    x *= r;
    y *= r;

    return { x, y };
}

void randomCircles(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint, Transform2D::SoA &transforms) {
    for (int i = 0; i < n; ++i) {
        glm::vec2 u = getRandomPoint();

        float u2 = dis(gen) * 0.05f;

        physicsWorld.add(new Circle{ u2 }, { u });
        Transform2D transform = { u, { u2, u2 }, 0.f };
        transforms.add(transform);
    }
}

void randomSquares(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint, Transform2D::SoA &transforms) {
    for (int i = 0; i < n; ++i) {
        glm::vec2 u = getRandomPoint();

        float u1 = dis(gen) * 0.1f + 0.02f;
        float u2 = dis(gen) * u1 + 0.01f;

        float theta = dis(gen) * pi * 180.0f;

        Physics2DState state{};
        state.p = u;
        state.theta = theta;
        physicsWorld.add(new Box{ { u1, u2 } }, state);
        Transform2D transform = { u, { u1, u2 }, theta };
        transforms.add(transform);
    }
}
