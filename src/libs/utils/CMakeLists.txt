cmake_minimum_required(VERSION 3.5)

project(utils)

set(CMAKE_CXX_STANDARD 11)

set(TARGET_NAME ${PROJECT_NAME})

file(GLOB SOURCES #
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" #
)

# dependencies
list(APPEND TARGET_DEPENDENCIES #
     "eigen" #
     "nlohmann_json::nlohmann_json" #
)

# target include dirs
list(APPEND TARGET_INCLUDE_DIRS #
     PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# target link libs
list(APPEND TARGET_LINK_LIBS #
     PUBLIC "eigen" #
     PUBLIC "nlohmann_json::nlohmann_json" #
)

# compile definitions
list(APPEND COMPILE_DEFINITIONS #
     PUBLIC "DATA_FOLDER=${DATA_FOLDER}" #
)

# create target
create_library(
    ${TARGET_NAME}
    "${SOURCES}" #
    "${TARGET_DEPENDENCIES}" #
    "${TARGET_INCLUDE_DIRS}" #
    "${TARGET_LINK_LIBS}" #
    "${COMPILE_DEFINITIONS}" #
)