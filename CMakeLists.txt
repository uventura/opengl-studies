cmake_minimum_required(VERSION 3.10)

project(gl-studies VERSION 1.0)

add_executable(${PROJECT_NAME} main.cpp)

#====================================
#           Dependencies
#====================================

# GLAD
add_subdirectory(deps/glad)
target_link_libraries(${PROJECT_NAME} glad)
target_include_directories(${PROJECT_NAME} PUBLIC deps/glad/include)

# GLFW
add_subdirectory(deps/glfw)
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PUBLIC deps/glfw/include deps/glfw/deps)

