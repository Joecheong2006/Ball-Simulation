#include "OrthoCamera.hpp"
#include "profiling.hpp"

#include "MainWindow.hpp"

OrthoCamera::OrthoCamera(glm::vec2 position, glm::vec2 viewportSize, float near, float far)
    : position(position)
{
    ZoneScoped;
    float aspect = (float)MainWindow::Width() / MainWindow::Height();
    projection = glm::ortho(-aspect * viewportSize.x, aspect * viewportSize.x,
            -viewportSize.y, viewportSize.y, near, far);
}
