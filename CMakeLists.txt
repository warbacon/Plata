cmake_minimum_required(VERSION 3.16)
project(Plata VERSION 0.1 LANGUAGES CXX)

# ==============================================================================
# Dependencies
# ==============================================================================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# ==============================================================================
# General CMake configuration
# ==============================================================================
qt_standard_project_setup()

# ==============================================================================
# Project files
# ==============================================================================
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

# ==============================================================================
# Build MuPDF as external dependency
# ==============================================================================
include(ExternalProject)

set(MUPDF_VERSION "1.26.11")
set(MUPDF_URL "https://casper.mupdf.com/downloads/archive/mupdf-${MUPDF_VERSION}-source.tar.gz")
set(MUPDF_HASH "67798ec73ba8b70c45893e07497a2107a551f05a")

# Set MuPDF directories
set(MUPDF_PREFIX "${CMAKE_BINARY_DIR}/mupdf-prefix")
set(MUPDF_SOURCE_DIR "${MUPDF_PREFIX}/src/mupdf")
set(MUPDF_INCLUDE_DIR "${MUPDF_SOURCE_DIR}/include")
set(MUPDF_LIB_DIR "${MUPDF_SOURCE_DIR}/build/release")
set(MUPDF_LIB_PATH "${MUPDF_LIB_DIR}/libmupdf.a")

# Create the include directory if it doesn't exist
file(MAKE_DIRECTORY ${MUPDF_INCLUDE_DIR})

ExternalProject_Add(
    mupdf
    PREFIX ${MUPDF_PREFIX}
    URL ${MUPDF_URL}
    URL_HASH SHA1=${MUPDF_HASH}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make libs
    BUILD_IN_SOURCE TRUE
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${MUPDF_LIB_PATH}
)

# Create imported library for MuPDF
add_library(mupdf_lib STATIC IMPORTED)
set_target_properties(mupdf_lib PROPERTIES
    IMPORTED_LOCATION ${MUPDF_LIB_PATH}
    INTERFACE_INCLUDE_DIRECTORIES ${MUPDF_INCLUDE_DIR}
)
add_dependencies(mupdf_lib mupdf)

# ==============================================================================
# Main executable
# ==============================================================================
qt_add_executable(Plata MANUAL_FINALIZATION ${PROJECT_SOURCES})

# Link libraries
target_link_libraries(Plata PRIVATE
    Qt6::Widgets
    mupdf_lib
)

# Executable properties
set_target_properties(Plata PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# ==============================================================================
# Installation
# ==============================================================================
install(TARGETS Plata
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ==============================================================================
# Finalization
# ==============================================================================
qt_finalize_executable(Plata)
