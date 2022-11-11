cmake_minimum_required(VERSION 3.5)

project(gui)

set(TARGET_NAME ${PROJECT_NAME})

set(SHADER_FOLDER
    "\"${CMAKE_CURRENT_SOURCE_DIR}/shaders\""
    CACHE STRING ""
)

file(
    GLOB
    SOURCES #
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" #
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets/*.cpp" #
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert" #
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag" #
)

# dependencies
list(
    APPEND
    TARGET_DEPENDENCIES #
    "glm" #
    "imgui" #
    "stb_image" #
    "nlohmann_json::nlohmann_json" #
    "tinyobjloader" #
    "eigen" #
    "crl::utils" #
)

# target include dirs
list(APPEND TARGET_INCLUDE_DIRS #
     PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" #
     PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets/include" #
)

# target link libs
list(
    APPEND
    TARGET_LINK_LIBS #
    PUBLIC
    "glm" #
    PUBLIC
    "imgui" #
    PUBLIC
    "stb_image" #
    PUBLIC
    "nlohmann_json::nlohmann_json" #
    PUBLIC
    "tinyobjloader" #
    PUBLIC
    "eigen" #
    PUBLIC
    "crl::utils" #
)

# compile definitions
list(
    APPEND
    COMPILE_DEFINITIONS #
    PUBLIC
    "DATA_FOLDER=${DATA_FOLDER}" #
    PUBLIC
    "SHADER_FOLDER=${SHADER_FOLDER}" #
    PUBLIC
    "IMGUI_FONT_FOLDER=${IMGUI_FONT_FOLDER}" #
    PUBLIC
    "IMGUI_IMPL_OPENGL_LOADER_GLAD" #
)

# we use filesystem which is "experimental" for some compilers for those we need
# to add fs lib to link libraries
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # gcc < 7: supports fs as experimentals
    # gcc = 8: supports fs but needs to link stdc++fs
    # gcc > 9: supports fs as std library
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        list(APPEND TARGET_LINK_LIBS PUBLIC "stdc++fs")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # msvc
    # TODO:
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # llvm clang
    # ~~~
    # llvm clang needs to link stdc++fs
    list(APPEND TARGET_LINK_LIBS PUBLIC "stdc++fs")
endif()

# create target
create_library(
    ${TARGET_NAME}
    "${SOURCES}" #
    "${TARGET_DEPENDENCIES}" #
    "${TARGET_INCLUDE_DIRS}" #
    "${TARGET_LINK_LIBS}" #
    "${COMPILE_DEFINITIONS}" #
)