#include "MainWindow.h"

#include <assert.h>
#include <iostream>

MainWindow MainWindow::mw = MainWindow();

void MainWindow::Initialize(int width, int height, const std::string &title) {
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

bool MainWindow::ShouldClose() {
    return glfwWindowShouldClose(mw.glfwWindow);
}

void MainWindow::SwapBuffers() {
    glfwSwapBuffers(mw.glfwWindow);
}
