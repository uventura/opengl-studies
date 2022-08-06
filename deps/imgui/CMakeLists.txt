file(GLOB CPP_FILES "./*.cpp")
file(GLOB H_FILES "./*.h")

add_library(
    imgui
    ${CPP_FILES}
    ${BACKEND_FILES}
    ${H_FILES}
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC .)
target_link_libraries(imgui glad)
target_link_libraries(imgui glfw)
