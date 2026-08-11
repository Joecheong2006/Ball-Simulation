cmake_minimum_required(VERSION 3.14)
project(GladTarget LANGUAGES C)

# Target the new v2 C source file
add_library(glad STATIC src/gl.c)

# Expose your local header directory paths
target_include_directories(glad PUBLIC include/)
