add_library(SimulationLib STATIC
    Resources.cpp

    gl/Buffer.cpp
    gl/Shader.cpp
    gl/ShaderProgram.cpp

    ${CMAKE_SOURCE_DIR}/lib/stb_image/stb_image.cpp
)


target_link_libraries(SimulationLib PUBLIC
    glad
    glfw
    glm::glm
)

target_include_directories(SimulationLib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
)

target_compile_definitions(SimulationLib PUBLIC PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

add_executable(Simulation main.cpp)
target_link_libraries(Simulation PRIVATE SimulationLib)
