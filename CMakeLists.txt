cmake_minimum_required(VERSION 3.5)
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "" FORCE)

project(RTLab)
option(GROUP_BY_EXPLORER ON)
option(USE_SOLUTION_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE INCLUDE_FILES  ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp ${CMAKE_SOURCE_DIR}/include/*.cc)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SHADERS_FILES ${CMAKE_SOURCE_DIR}/shaders/*.glsl ${CMAKE_SOURCE_DIR}/shaders/*.hlsl)

set(AllFile ${INCLUDE_FILES} ${SOURCE_FILES} ${SHADERS_FILES})

foreach (fileItem ${AllFile})
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach ()

find_package(OpenMP REQUIRED)
if (OpenMP_FOUND)
    message("OpenMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(OpenCV_DIR B:/repos/3dparty/opencv/build_win_library/install/x64/vc16/lib)
set(OpenCV_INCLUDE_DIR B:/repos/3dparty/opencv/build_win_library/install/x64/vc16/include)

set(DLL_DIR ${PROJECT_SOURCE_DIR}/libs)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARIES})

find_package(OpenCV CONFIG REQUIRED)
if (OpenCV_FOUND)
    message("OpenCV FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenCV_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenCV_CXX_FLAGS}")
endif ()

# 解决CLion + MSVC 下的字符编码问题
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME} ${AllFile})

set_property(SOURCE ${SHADER_FILES} PROPERTY VS_TOOL_OVERRIDE "shader")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

if (OpenCV_FOUND)
    # ${CMAKE_BUILD_TYPE}
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
    file(COPY ${DLL_DIR}/ DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    message(${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
endif()


# 统一输出目录
# 统一libs
# 统一bin