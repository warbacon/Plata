cmake_minimum_required(VERSION 3.16)
project(Plata VERSION 0.1 LANGUAGES CXX)

# ==============================================================================
# General CMake configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt configuration
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ==============================================================================
# Dependencies
# ==============================================================================
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ==============================================================================
# Project files
# ==============================================================================
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# ==============================================================================
# Build MuPDF as external dependency
# ==============================================================================
include(ExternalProject)

set(MUPDF_VERSION "1.26.11")
set(MUPDF_URL "https://casper.mupdf.com/downloads/archive/mupdf-${MUPDF_VERSION}-source.tar.gz")
set(MUPDF_HASH "67798ec73ba8b70c45893e07497a2107a551f05a")

ExternalProject_Add(
    mupdf
    URL ${MUPDF_URL}
    URL_HASH SHA1=${MUPDF_HASH}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make libs
    BUILD_IN_SOURCE TRUE
    INSTALL_COMMAND ""
)

# MuPDF build directory
ExternalProject_Get_Property(mupdf SOURCE_DIR)
set(MUPDF_INCLUDE_DIR "${SOURCE_DIR}/include")
set(MUPDF_LIB_DIR "${SOURCE_DIR}/build/release")

# ==============================================================================
# Main executable
# ==============================================================================
qt_add_executable(Plata MANUAL_FINALIZATION ${PROJECT_SOURCES})

# Dependencies
add_dependencies(Plata mupdf)

# Include directories
target_include_directories(Plata PRIVATE ${MUPDF_INCLUDE_DIR})

# Link libraries
target_link_libraries(Plata PRIVATE
    Qt6::Widgets
    ${MUPDF_LIB_DIR}/libmupdf.a
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
include(GNUInstallDirs)

install(TARGETS Plata
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ==============================================================================
# Finalization
# ==============================================================================
qt_finalize_executable(Plata)
