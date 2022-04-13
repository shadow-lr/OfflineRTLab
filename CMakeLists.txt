cmake_minimum_required(VERSION 3.5)

project(rtTheRestOfYourLife)
option(GROUP_BY_EXPLORER ON)    # 启用保留文件结构和资源管理器一样
option(USE_SOLUTION_FOLDERS ON)# 允许对项目文件按文件夹分类

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE "Release")

include_directories(${CMAKE_SOURCE_DIR}/include)  # 让 CMake 找到头文件
#link_directories(${CMAKE_SOURCE_DIR}/lib)  # 让CMake找到库文件

file(GLOB_RECURSE INCLUDE_FILES  ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SHADERS_FILES ${CMAKE_SOURCE_DIR}/shaders/*.glsl ${CMAKE_SOURCE_DIR}/shaders/*.hlsl)

# 对 AllFile 变量里面的所有文件分类(保留资源管理器的目录结构)
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

# 解决CLion + MSVC 下的字符编码问题
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME} ${AllFile})

set_property(SOURCE ${SHADER_FILES} PROPERTY VS_TOOL_OVERRIDE "shader")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
