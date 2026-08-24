#pragma once

#include <glad/gl.h> 
#include <GLFW/glfw3.h>
#include <string_view>

struct MainWindow {
    static void Initialize(int width, int height, std::string_view title);
    static bool ShouldClose();
    static void SwapBuffers();
    static int Width() { return mw.width; }
    static int Height() { return mw.height; }

private:
    int width, height;
    GLFWwindow *glfwWindow = nullptr;
    static MainWindow mw;

};
