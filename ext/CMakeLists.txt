# TODO:
#
# what if a dependency conflicts? (e.g. json for DDE and json for crl-core)
#
# * do we download json twice and use one for crl-core and one for dde?
# * or just download json once and use it for both crl-core and dde?
#
# at the moment we are doing option 2 but maybe option 1 is more reasonable

# FetchContent command is available with cmake >= 3.11
include(FetchContent)

# -----------------------------------------------------------------------------
# helper funtion to download a 'fetch content declare'
# -----------------------------------------------------------------------------

macro(fetch what)
    FetchContent_GetProperties("${what}")
    if(NOT ${${what}_POPULATED})
        message(STATUS "fetching ${what} ...")
        FetchContent_Populate(${what})
    endif()
    mark_as_advanced(${${what}_SOURCE_DIR})
endmacro()

# -----------------------------------------------------------------------------
# dependencies for utils
# -----------------------------------------------------------------------------

# Eigen library
#
# download as zip, so it's a bit smaller than downloading the whole repo
FetchContent_Declare(
    eigen #
    URL https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.zip #
    URL_HASH MD5=888aab45512cc0c734b3e8f60280daba #
)
fetch(eigen)
add_library(eigen INTERFACE)
add_library (Eigen3::Eigen ALIAS eigen)
target_include_directories(eigen INTERFACE ${eigen_SOURCE_DIR})

# -----------------------------------------------------------------------------
# json
FetchContent_Declare(
    json #
    URL https://github.com/nlohmann/json/releases/download/v3.7.3/include.zip #
    URL_HASH MD5=fb96f95cdf609143e998db401ca4f324 #
)
fetch(json)

# this is a bit mess... (alias name on DDE cmake has some typo)
if(TARGET nlohmann_json)
    if(NOT TARGET nlohmann_json::nlohmann_json)
        add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
    endif(NOT TARGET nlohmann_json::nlohmann_json)
else()
    add_library(nlohmann_json INTERFACE)
    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
    target_include_directories(
        nlohmann_json INTERFACE $<BUILD_INTERFACE:${json_SOURCE_DIR}/include>
    )
endif()

# -----------------------------------------------------------------------------
# dependencies for gui
# -----------------------------------------------------------------------------

# glad
set(OpenGL_GL_PREFERENCE GLVND)
add_subdirectory(glad)

# -----------------------------------------------------------------------------
# glfw
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/3.3.2.zip
    URL_HASH MD5=69d0aa90a9278c0dd8be975d1ae4aaf2
    UPDATE_COMMAND "" CONFIGURE_COMMAND "" BUILD_COMMAND "" INSTALL_COMMAND ""
)
fetch(glfw)

# build options
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "GLFW lib only"
)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "GLFW lib only"
)
set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "GLFW lib only"
)
set(GLFW_INSTALL
    OFF
    CACHE BOOL "GLFW lib only"
)

add_subdirectory(${glfw_SOURCE_DIR} glfw)

# -----------------------------------------------------------------------------
# glm
FetchContent_Declare(
    glm #
    URL https://github.com/g-truc/glm/releases/download/0.9.9.7/glm-0.9.9.7.zip #
    URL_HASH MD5=da4740b5b41a1e2ccb2955af74e7c298 #
)
fetch(glm)
set(GLM_TEST_ENABLE
    OFF
    CACHE BOOL ""
)
add_subdirectory(${glm_SOURCE_DIR} glm)
target_include_directories(glm INTERFACE ${glm_SOURCE_DIR}/glm>)

# -----------------------------------------------------------------------------
# imgui
#
# imgui for graphical user interface. since imgui is not a cmake project, we
# need to add target by our own. we use imgui implementation for glfw and
# opengl3

FetchContent_Declare(
    imgui #
    GIT_REPOSITORY https://github.com/ocornut/imgui.git #
    GIT_TAG v1.78 #
)
fetch(imgui)

file(
    GLOB
    IMGUI_SRC #
    ${imgui_SOURCE_DIR}/*.cpp #
    ${imgui_SOURCE_DIR}/examples/imgui_impl_glfw.cpp #
    ${imgui_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp #
)

add_library(imgui STATIC ${IMGUI_SRC})

set_target_properties(imgui PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    imgui PUBLIC ${imgui_SOURCE_DIR} #
                 ${imgui_SOURCE_DIR}/examples #
)

target_link_libraries(
    imgui PUBLIC glfw #
                 glad #
)

set(IMGUI_FONT_FOLDER
    "\"${imgui_SOURCE_DIR}/misc/fonts\""
    CACHE STRING ""
)

# -----------------------------------------------------------------------------
# stb_image
add_subdirectory(stb_image)

# -----------------------------------------------------------------------------
# tinyobjloader
FetchContent_Declare(
    tinyobjloader #
    URL https://github.com/tinyobjloader/tinyobjloader/archive/v2.0-rc1.zip #
    URL_HASH MD5=f660a9f91fc2ea5c4f33d7530f6a69bc #
)
fetch(tinyobjloader)
add_subdirectory(${tinyobjloader_SOURCE_DIR} tinyobjloader)

# -----------------------------------------------------------------------------
# dependencies for simAndControl
# -----------------------------------------------------------------------------

# ode
if(CRL_BUILD_ODE)
    FetchContent_Declare(
        ode #
        GIT_REPOSITORY https://gitlab.inf.ethz.ch/scoros/ode #
        GIT_TAG 0.16.1 #
    )
    fetch(ode)

    # build options
    set(ODE_WITH_DEMOS
        OFF
        CACHE BOOL "ODE lib only"
    )
    set(ODE_WITH_OPCODE
        OFF
        CACHE BOOL "ODE lib only"
    )
    set(ODE_WITH_TESTS
        OFF
        CACHE BOOL "ODE lib only"
    )
    add_subdirectory(${ode_SOURCE_DIR} ode)
endif(CRL_BUILD_ODE)

# -----------------------------------------------------------------------------
# dependencies for optimization
# -----------------------------------------------------------------------------

# lbfgspp
FetchContent_Declare(
    lbfgspp #
    GIT_REPOSITORY https://github.com/yixuan/LBFGSpp #
    GIT_TAG f047ef4586869855f00e72312e7b4d78d11694b1 #
)
fetch(lbfgspp)
add_library(lbfgspp INTERFACE)
target_include_directories(
    lbfgspp INTERFACE $<BUILD_INTERFACE:${lbfgspp_SOURCE_DIR}/include>
)

if(CRL_BUILD_MODERN_QP)
    # -------------------------------------------------------------------------
    # osqp
    FetchContent_Declare(
        osqpcpp #
        GIT_REPOSITORY https://github.com/google/osqp-cpp.git #
        GIT_TAG 10a82a98ae88f84c54bfdae683691b994561ec43 #
    )
    fetch(osqpcpp)

    set(OSQP-CPP_BUILD_TESTS
        OFF
        CACHE BOOL "OSQP lib only"
    )

    add_subdirectory(${osqpcpp_SOURCE_DIR} osqpcpp)

    # -------------------------------------------------------------------------
    # qpmad
    # FetchContent_Declare(
    #     qpmad #
    #     GIT_REPOSITORY https://github.com/asherikov/qpmad.git #
    #     GIT_TAG ros-1.0.2 #
    # )
    # fetch(qpmad)
    # add_subdirectory(${qpmad_SOURCE_DIR} qpmad)
endif(CRL_BUILD_MODERN_QP)