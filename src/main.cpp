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

#include "gl/BufferLayout.h"

#include "MainWindow.h"

struct Circle {
    glm::vec2 origin;
    float radius;
};

struct Square {
    glm::vec2 size;
};

struct Physics2DState {
    glm::vec2 p, v, a;
    float theta;
};

struct Physics2DStates {
    std::vector<glm::vec2> p, v, a;
    std::vector<float> theta;
};

struct Physics2DWorld {
    std::vector<Circle> circles;
    std::vector<Square> squares;
    Physics2DStates physicsStates;
    int N = 0;

    void addPhysicsState(const Physics2DState &state) {
        physicsStates.p.push_back(state.p);
        physicsStates.v.push_back(state.v);
        physicsStates.a.push_back(state.a);
        physicsStates.theta.push_back(state.theta);
    }

    void addCircle(const Circle &circle, const Physics2DState &state = {}) {
        ZoneScoped;
        circles.push_back(circle);
        addPhysicsState(state);
        ++N;
    }

    void addSquare(const Square &square, const Physics2DState &state = {}) {
        ZoneScoped;
        squares.push_back(square);
        addPhysicsState(state);
        ++N;
    }

    void updatePhysicsStates(float dt) {
        ZoneScoped;
        float dt2 = dt * dt;

        for (std::size_t i = 0; i < N; ++i) {
            const auto &a = physicsStates.a[i] = glm::vec2(0.0f, -9.81f);
            const auto &v = physicsStates.v[i] += a * dt;
            physicsStates.p[i] += 0.5f * a * dt2 + v * dt;
        }
    }

    void updatePhysics(float dt) {
        ZoneScoped;
        updatePhysicsStates(dt);
    }
};

struct World {
};

#include "RenderMesh.h"
#include "RenderMaterial.h"

#include "OrthoCamera.h"

struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
};

#include "RenderObjects.h"
#include "Renderer.h"

using FunctorGetRandom = glm::vec2(*)();
Transform2D::Container randomCircles(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint);
Transform2D::Container randomSquares(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint);

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

    int circleCount = 1000;
    Transform2D::Container circleTransforms = randomCircles(circleCount, physicsWorld, randomPointFromHeart);

    int squareCount = 500;
    Transform2D::Container transforms = randomSquares(squareCount, physicsWorld, randomPointFromHeart);

    while (!MainWindow::ShouldClose()) {
        FrameMark;

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        physicsWorld.updatePhysics(1.f / 1200);

        { ZoneScopedN("Sync physics states");
            for (auto i = 0; i < circleCount; ++i) {
                circleTransforms.setPositionAt(i, physicsWorld.physicsStates.p[i]);
                circleTransforms.setAngleAt(i, physicsWorld.physicsStates.theta[i]);
            }
            for (auto i = 0; i < squareCount; ++i) {
                transforms.setPositionAt(i, physicsWorld.physicsStates.p[i + circleCount]);
                transforms.setAngleAt(i, physicsWorld.physicsStates.theta[i + circleCount]);
            }
        }

        FrameMarkStart("Render");
        renderer.submitBatch(quatId, blueMaterialId, transforms);
        renderer.submitBatch(quatId, circleMaterialId, circleTransforms);
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

Transform2D::Container randomCircles(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint) {
    Transform2D::Container transforms{};

    for (int i = 0; i < n; ++i) {
        glm::vec2 u = getRandomPoint();

        float u2 = dis(gen) * 0.05f;

        physicsWorld.addCircle({ { 0, 0 }, u2 }, { u });
        Transform2D transform = { u, { u2, u2 }, 0.f };
        transforms.add(transform);
    }

    return transforms;
}

Transform2D::Container randomSquares(int n, Physics2DWorld &physicsWorld, FunctorGetRandom getRandomPoint) {
    Transform2D::Container transforms{};

    for (int i = 0; i < n; ++i) {
        glm::vec2 u = getRandomPoint();

        float u1 = dis(gen) * 0.1f + 0.02f;
        float u2 = dis(gen) * u1 + 0.01f;

        float theta = dis(gen) * pi * 180.0f;

        physicsWorld.addSquare({ { u1, u2 } }, { u, {}, {}, theta });
        Transform2D transform = { u, { u1, u2 }, theta };
        transforms.add(transform);
    }

    return transforms;
}
