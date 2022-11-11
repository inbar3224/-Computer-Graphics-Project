cmake_minimum_required(VERSION 3.5)

project(fancy_app)

file(GLOB SOURCES #
     "*.h" #
     "*.cpp" #
)

list(APPEND TARGET_DEPENDENCIES #
     "crl::utils" #
     "crl::gui" #
)

list(APPEND TARGET_INCLUDE_DIRS #
     PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

list(APPEND TARGET_LINK_LIBS #
     PUBLIC "crl::utils" #
     PUBLIC "crl::gui" #
)

list(APPEND COMPILE_DEFINITIONS #
     PUBLIC "DATA_FOLDER=${DATA_FOLDER}" #
)

create_app(
    ${PROJECT_NAME}
    "${SOURCES}" #
    "${TARGET_DEPENDENCIES}" #
    "${TARGET_INCLUDE_DIRS}" #
    "${TARGET_LINK_LIBS}" #
    "${COMPILE_DEFINITIONS}"
)
