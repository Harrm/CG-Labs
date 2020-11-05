project(CG_Labs CXX)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PREV_TARGET "")
foreach(TARGET clear_rendertarget draw_line projections triangle_rasterization  depth_buffer)
    add_library (${TARGET} src/${TARGET}.cpp)
    target_include_directories(${TARGET} PUBLIC src/ lib/linalg lib/stb lib/tinyobjloader)
    target_link_libraries(${TARGET} PUBLIC ${PREV_TARGET})
    message("Linking ${PREV_TARGET} to ${TARGET}")

    add_executable(${TARGET}_exec src/${TARGET}_main.cpp)
    target_link_libraries(${TARGET}_exec ${TARGET})
    message("Linking ${TARGET} to ${TARGET}_exec")
    set(PREV_TARGET ${TARGET})
endforeach()
