#include <glad/gl.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image/stb_image.h"

#include <vector>
#include <string>
#include <assert.h>
#include <random>

#include <chrono>

#define SOA_TRANSFORM 1

struct ScopeTimer {
    std::chrono::high_resolution_clock::time_point start
        = std::chrono::high_resolution_clock::now();

    ~ScopeTimer() {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        std::cout << elapsed.count() * 1e-6 << "ms\n";
    }

};

#define SCOPE_TIMER() ScopeTimer ____

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
};

Vertex squareVertices[] = {
    // Positions    // Texture coords
    { {  0.5f,  0.5f  }, { 1.0f, 1.0f } },
    { {  0.5f, -0.5f  }, { 1.0f, 0.0f } },
    { { -0.5f, -0.5f  }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f  }, { 0.0f, 1.0f } },
};

unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 2) in vec2 aInstancePos;
layout (location = 3) in vec2 aInstanceScale;
layout (location = 4) in float aInstanceAngle;

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

#include "gl/Buffer.h"
#include "gl/BufferLayout.h"
#include "gl/ShaderProgram.h"

struct MainWindow {
    static inline void Initialize(int width, int height, const std::string &title) {
        mw.glfwWindow = glfwCreateWindow(width, height, "", NULL, NULL);
        assert(mw.glfwWindow);
        glfwMakeContextCurrent(mw.glfwWindow);

        // GLAD 2 initialization syntax
        int version = gladLoadGL(glfwGetProcAddress); 
        if (version == 0) {
            std::cerr << "Failed to initialize GLAD 2 context" << std::endl;
            assert(version);
        }

        std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) 
                  << "." << GLAD_VERSION_MINOR(version) << std::endl;

        glfwGetFramebufferSize(mw.glfwWindow, &mw.width, &mw.height);
        glViewport(0, 0, mw.width, mw.height);
    }

    static inline int Width() { return mw.width; }
    static inline int Height() { return mw.height; }
    static inline bool ShouldClose() { return glfwWindowShouldClose(mw.glfwWindow); }
    static inline void SwapBuffers() { glfwSwapBuffers(mw.glfwWindow); }

private:
    int width, height;
    GLFWwindow *glfwWindow = nullptr;
    static MainWindow mw;

};

MainWindow MainWindow::mw = MainWindow();

struct OrthoCamera {
    glm::mat4 projection;
    glm::vec2 position;

    OrthoCamera(glm::vec2 position, glm::vec2 viewportSize, float near, float far)
        : position(position)
    {
        float aspect = (float)MainWindow::Width() / MainWindow::Height();
        projection = glm::ortho(-aspect * viewportSize.x, aspect * viewportSize.x,
                -viewportSize.y, viewportSize.y, near, far);
    }

};

struct Transform2DSoA {
    std::vector<glm::vec2> positions, scales;
    std::vector<float> angles;
};

struct Transform2D {
    glm::vec2 position, scale;
    float angle;

    glm::mat4 toMat4() {
        float c = std::cos(angle);
        float s = std::sin(angle);
        glm::mat4 m(1.0f);
        m[0] = glm::vec4(c * scale.x, s * scale.x, 0.0f, 0.0f);
        m[1] = glm::vec4(-s * scale.y, c * scale.y, 0.0f, 0.0f);
        m[3] = glm::vec4(position.x, position.y, 0.0f, 1.0f);
        return m;
    }
};

struct Circle {
    glm::vec2 origin;
    float radius;
};

struct Physics2DState {
    glm::vec2 p, v, a;
    float theta;
};

struct Physics2DStateSoA {
    std::vector<glm::vec2> p, v, a;
    std::vector<float> theta;
};

struct Physics2DWorld {
    std::vector<Circle> circles;
    Physics2DStateSoA physicsStatesSoA;
    int N = 0;

    std::size_t addCircle(const Circle &circle, const Physics2DState &state = {}) {
        circles.push_back(circle);

        physicsStatesSoA.p.push_back(state.p);
        physicsStatesSoA.v.push_back(state.v);
        physicsStatesSoA.a.push_back(state.a);
        physicsStatesSoA.theta.push_back(state.theta);
        return ++N;
    }

    void updatePhysicsStates(float dt) {
        float dt2 = dt * dt;

        for (std::size_t i = 0; i < N; ++i) {
            const auto &a = physicsStatesSoA.a[i] = glm::vec2(0.0f, -9.81f);
            const auto &v = physicsStatesSoA.v[i] += a * dt;
            physicsStatesSoA.p[i] += 0.5f * a * dt2 + v * dt;
        }
    }

    void updatePhysics(float dt) {
        updatePhysicsStates(dt);
    }
};

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
    // VBO
    //

    // Create a GLBuffer
    gl::Buffer Vbo(GL_ARRAY_BUFFER);

    // Bind the buffer with the type of GL_ARRAY_BUFFER
    Vbo.bind();

    // Store the data into the buffer we just bound which is Vbo in this case
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    Vbo.setData(sizeof(squareVertices), squareVertices);

    // 
    // EBO
    //

    // Create a GLBuffer
    gl::Buffer Ebo(GL_ELEMENT_ARRAY_BUFFER);

    // Bind the buffer with the type of GL_ARRAY_BUFFER
    // A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
    // So, don't unbind the EBO when VAO is bound otherwise it will have incorrect config.
    Ebo.bind();
     
    // Store the indices' data into the buffer we just bound
    Ebo.setData(sizeof(indices), indices);

    //
    // VAO
    //

    gl::BufferLayout layout;
    layout.bind();

    // Bind EBO so that VAO can related it
    Ebo.bind();

    // Bind VBO so that we can set vertex attribute
    Vbo.bind();
    layout.set(gl::BufferLayout::Aggregate<Vertex, float>());

    gl::Buffer instancePosVbo(GL_ARRAY_BUFFER);
    instancePosVbo.bind();

#if SOA_TRANSFORM
    layout.set<float>(0, 2, 2, 1);

    gl::Buffer instanceScaleVbo(GL_ARRAY_BUFFER);
    instanceScaleVbo.bind();
    layout.set<float>(0, 2, 2, 1);

    gl::Buffer instanceAngleVbo(GL_ARRAY_BUFFER);
    instanceAngleVbo.bind();
    layout.set<float>(0, 2, 2, 1);
#else
    layout.set(gl::BufferLayout::Aggregate<Transform2D, float>(), 1);
#endif

    // Expliclty tells Vao the layout of the data we stored into Vbo.
    // Vao contains the info of VBOs and their corresponding attribute (or layout).
    //
    // In order to set the attribute to the correct buffer we should use these two
    // functions when the target buffer is bound.
    //
    // 1. location of the attribute which should match
    //    the location in the vertexShaderSource.
    // 2. the size of the vertex attribute in this case it is 3
    // 3. the type of the data
    // 4. this is a boolean for if we want OpenGL to normalized our data to 0 to 1
    // 5. This is the offset of where the layout begin in this case it is just 0
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Enable the attribute given the location.
    // This enable us we this attribute in the shader.
    // glEnableVertexAttribArray(0);

    // Attribute for color at location 1
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    //
    // Shader Program
    //

    gl::ShaderProgram shaderProgram;

    {
        gl::Shader vertex(GL_VERTEX_SHADER, vertexShaderSource),
                 fragment(GL_FRAGMENT_SHADER, fragmentShaderSource);

        vertex.compile();
        fragment.compile();

        shaderProgram.attachShader(vertex);
        shaderProgram.attachShader(fragment);
        shaderProgram.link();
    }

    // 
    // Texture
    //

    // 
    // 2D camrea creation
    //

    OrthoCamera camera({ 0, 0 }, { 1.0f, 1.0f }, -1.0f, 1.0f);

    //
    // Set up shader uniforms
    //

    shaderProgram.use();
    shaderProgram.setUniformMat4("projection", camera.projection);
    shaderProgram.setUniform2f("camPos", camera.position);

    // 
    // Enable Transparent
    //

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 
    // Initialize instnaces' circle
    //

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    int N = 3'000'000;

#if SOA_TRANSFORM
    Transform2DSoA transforms;
#else
    std::vector<Transform2D> transforms;
    transforms.reserve(N);
#endif

    Physics2DWorld physicsWorld;

    for (std::size_t i = 0; i < N; ++i) {
        float u1 = dis(gen) * 2.0f * 3.14156f;

        float x = 16.f * std::pow(sin(u1), 3.f);
        float y = 13.f * cos(u1) - 5.f * cos(2.f * u1) - 2.f * cos(3.f * u1) - cos(4.f * u1);

        float scale = 0.06f;

        x *= scale;
        y *= scale;

        float r = dis(gen);

        x *= r;
        y *= r;

        float u2 = dis(gen) * 0.005f;

        physicsWorld.addCircle({ { 0, 0 }, u2 }, { { x, y } });
#if SOA_TRANSFORM
        transforms.positions.push_back({ x, y });
        transforms.scales.push_back({ u2, u2 });
        transforms.angles.push_back(0.f);
#else
        transforms.push_back({ { x, y }, { u2, u2 }, 0.f });
#endif
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!MainWindow::ShouldClose()) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        { SCOPE_TIMER();
            physicsWorld.updatePhysics(1.f / 120);
            std::printf("%-50s", "updatePhysics");
        }

        { SCOPE_TIMER();
#if SOA_TRANSFORM
            transforms.positions = physicsWorld.physicsStatesSoA.p;
            transforms.angles = physicsWorld.physicsStatesSoA.theta;
#else
            for (std::size_t i = 0; i < N; ++i) {
                transforms[i].position = physicsWorld.physicsStatesSoA.p[i];
                transforms[i].angle = physicsWorld.physicsStatesSoA.theta[i];
            }
#endif
            std::printf("%-50s", "physics state to instance buffer");
        }

        { SCOPE_TIMER();
            instancePosVbo.bind();
#if SOA_TRANSFORM
            instancePosVbo.setData(transforms.positions.size() * sizeof(glm::vec2), transforms.positions.data());
            instanceScaleVbo.bind();
            instanceScaleVbo.setData(transforms.scales.size() * sizeof(glm::vec2), transforms.scales.data());
            instanceAngleVbo.bind();
            instanceAngleVbo.setData(transforms.angles.size() * sizeof(float), transforms.angles.data());
#else
            instancePosVbo.setData(transforms.size() * sizeof(Transform2D), transforms.data());
#endif
            std::printf("%-50s", "loadTransformsToGPUBuffer");
        }

        shaderProgram.use();
        layout.bind();

        // glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, N); 
        { SCOPE_TIMER();
            glDrawElementsInstanced(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)0, N);
            std::printf("%-50s", "glDrawElementsInstanced");
        }

        { SCOPE_TIMER();
            MainWindow::SwapBuffers();
            std::printf("%-50s", "MainWindow::SwapBuffers");
        }
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
