#include "MainWindow.h"
#include "profiling.h"

#include <assert.h>
#include <iostream>

MainWindow MainWindow::mw = MainWindow();

void MainWindow::Initialize(int width, int height, std::string_view title) {
    ZoneScoped;
    mw.glfwWindow = glfwCreateWindow(width, height, title.data(), NULL, NULL);
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

bool MainWindow::ShouldClose() {
    ZoneScoped;
    return glfwWindowShouldClose(mw.glfwWindow);
}

void MainWindow::SwapBuffers() {
    ZoneScoped;
    glfwSwapBuffers(mw.glfwWindow);
}
