find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

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
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(SimulationLib PUBLIC PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
