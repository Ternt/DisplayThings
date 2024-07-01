cmake_minimum_required(VERSION 3.20)
project(DTEngine)

add_executable(DTEngine 
  "src/Main.cpp"
)

message("Top level CMakeLists.txt")
add_subdirectory(vendor/glfw)
