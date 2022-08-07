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

# IMGUI
add_subdirectory(deps/imgui)
target_link_libraries(${PROJECT_NAME} imgui)
target_include_directories(${PROJECT_NAME} PUBLIC deps/imgui deps/imgui/backends)

# STB IMAGE
add_subdirectory(deps/stb_image)
target_link_libraries(${PROJECT_NAME} stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC deps/stb_image)

# GLM
add_subdirectory(deps/glm)
target_link_libraries(${PROJECT_NAME} glm)
target_include_directories(${PROJECT_NAME} PUBLIC deps/glm)

#====================================
#           Libraries
#====================================

# MainLibrary
add_subdirectory(libs)
target_link_libraries(${PROJECT_NAME} mainlib)
target_include_directories(${PROJECT_NAME} PUBLIC libs)
