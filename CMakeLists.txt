# follows semantic versioning conventions for versions
# https://semver.org/
project(OpenGL_vim VERSION 1.0.0.0 LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)


add_library(WinModule "../lib/glad.c")

add_executable(OpenGL_vim "./main.cpp")
target_link_libraries(OpenGL_vim WinModule)
