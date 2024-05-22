cmake_minimum_required(VERSION 3.26)
project(all)

#
set(app_name test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{FmDev}/website/${app_name}/)

#
file(GLOB app_src
    ./*.cpp
    ./*.hpp
    ./*.h
    ./lua-5.4.4/*.hpp
    ./lua-5.4.4/*.h
    ./lua-5.4.4/*.c
    ./lua-5.4.4/*.cpp
)

#
MESSAGE(STATUS "APP SRC IS:"
    ${app_src}
)
#
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# 生成
add_executable(${app_name}
    ${app_src}
)

set_property(TARGET ${app_name} PROPERTY CXX_STANDARD 17)

target_compile_definitions(${app_name} PUBLIC
    __EMSCRIPTEN__
    EMSCRIPTEN
    LUA_VERSION_NUM=504
    LUABRIDGE_CXX17=1
)

# app源码原始包中的include
target_include_directories(${app_name} PUBLIC
    ./lua-5.4.4
    ./
)


# link flag
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS}

    $ENV{fm_links_exe_flags}
)


