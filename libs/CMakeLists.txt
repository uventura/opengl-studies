file(GLOB LIB_HEADERS "./*.hpp")
file(GLOB LIB_SOURCES "./*.cpp")

add_library(mainlib ${LIB_HEADERS} ${LIB_SOURCES})

#==========================
#       Dependencies
#==========================

target_link_libraries(mainlib glad)
target_link_libraries(mainlib glfw)
