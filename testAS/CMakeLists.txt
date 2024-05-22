cmake_minimum_required(VERSION 3.26)
project(all)

#
set(app_name $ENV{app_name})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{FmDev}/website/${app_name}/)

#
file(GLOB app_src
    ./as-exp/*.cpp
    ./AngelScript/source/*.cpp
    ./AngelScript/add_on/autowrapper/generator/*.cpp
    ./AngelScript/add_on/contextmgr/*.cpp
    ./AngelScript/add_on/datetime/*.cpp
    ./AngelScript/add_on/debugger/*.cpp
    ./AngelScript/add_on/scriptany/*.cpp
    ./AngelScript/add_on/scriptarray/*.cpp
    ./AngelScript/add_on/scriptbuilder/*.cpp
    ./AngelScript/add_on/scriptdictionary/*.cpp
    ./AngelScript/add_on/scriptfile/*.cpp
    ./AngelScript/add_on/scriptgrid/*.cpp
    ./AngelScript/add_on/scripthandle/*.cpp
    ./AngelScript/add_on/scripthelper/*.cpp
    ./AngelScript/add_on/scriptmath/*.cpp
    ./AngelScript/add_on/scriptstdstring/*.cpp
    ./AngelScript/add_on/serializer/*.cpp
    ./AngelScript/add_on/weakref/*.cpp
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
    AS_MAX_PORTABILITY=AS_MAX_PORTABILITY
)

# app源码原始包中的include
target_include_directories(${app_name} PUBLIC
    $ENV{FmDev}/${app_name}/as-exp
    $ENV{FmDev}/${app_name}/AngelScript/add_on
    $ENV{FmDev}/${app_name}/AngelScript/add_on/autowrapper/generator
    $ENV{FmDev}/${app_name}/AngelScript/add_on/autowrapper
    $ENV{FmDev}/${app_name}/AngelScript/add_on/contextmgr
    $ENV{FmDev}/${app_name}/AngelScript/add_on/datetime
    $ENV{FmDev}/${app_name}/AngelScript/add_on/debugger
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptany
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptarray
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptbuilder
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptdictionary
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptfile
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptgrid
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scripthandle
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scripthelper
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptmath
    $ENV{FmDev}/${app_name}/AngelScript/add_on/scriptstdstring
    $ENV{FmDev}/${app_name}/AngelScript/add_on/serializer
    $ENV{FmDev}/${app_name}/AngelScript/add_on/weakref
    $ENV{FmDev}/${app_name}/AngelScript/include
    $ENV{FmDev}/${app_name}/AngelScript/source
    $ENV{FmDev}/${app_name}/AngelScript
)


# link flag
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS}

    $ENV{fm_links_exe_flags}
)

# # link oflib
# target_link_libraries(${app_name} PUBLIC
    
#     $ENV{FmDev}/test3/libs/liblua54.a
# )
