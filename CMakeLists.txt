cmake_minimum_required(VERSION 3.5)

project(RTLab)
option(GROUP_BY_EXPLORER ON)
option(USE_SOLUTION_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE INCLUDE_FILES  ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp ${CMAKE_SOURCE_DIR}/include/*.cc)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/*.cpp)

set(AllFile ${INCLUDE_FILES} ${SOURCE_FILES} ${SHADERS_FILES})

foreach (fileItem ${AllFile})
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach ()

# OpenMp
find_package(OpenMP REQUIRED)
if (OpenMP_FOUND)
    message("OpenMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# CLion + MSVC
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME} ${AllFile})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(DLL_DIR ${PROJECT_SOURCE_DIR}/libs)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/Debug)
    file(COPY ${DLL_DIR}/Debug/ DESTINATION ${EXECUTABLE_OUTPUT_PATH})
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/Release)
    file(COPY ${DLL_DIR}/Release/ DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif()

# assets
#file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})
# 判断某个文件夹不存在
