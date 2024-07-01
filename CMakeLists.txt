#[===[ Top level CMakeLists.txt

  .
  ├── bin 
  ├── Project 1 
  │   ├── src
  │   └── dependencies
  │       └── glad 
  │       └── glfw
  │       └── imgui 
  ├── README.md
  └── dependencies

#]===]




cmake_minimum_required(VERSION 3.20)
project(DTEngine)

message("Top level CMakeLists.txt")
add_subdirectory(DisplayThings)
