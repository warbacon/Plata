cmake_minimum_required(VERSION 3.28)
project(Plata VERSION 0.1.0 LANGUAGES CXX)

# ==============================================================================
# Dependencies
# ==============================================================================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MUPDF REQUIRED IMPORTED_TARGET mupdf)

# ==============================================================================
# CMake configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
qt_standard_project_setup()

# ==============================================================================
# Project files
# ==============================================================================
set(PROJECT_SOURCES
    src/main.cpp
    src/pdfviewer.cpp
    src/pdfviewer.h
    src/pdfrenderer.cpp
    src/pdfrenderer.h
    src/pdfviewer.ui
    src/resources.qrc
)

# ==============================================================================
# Main executable
# ==============================================================================
qt_add_executable(plata MANUAL_FINALIZATION ${PROJECT_SOURCES})

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Link libraries
target_link_libraries(plata PRIVATE
    Qt6::Widgets
    PkgConfig::MUPDF
)

# Link-Time Optimization
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported)
    set_target_properties(plata PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "IPO/LTO enabled")
else()
    message(STATUS "IPO/LTO not supported: ${ipo_error}")
endif()

# Executable properties
set_target_properties(plata PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.warbacon.plata
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_BUNDLE_NAME "Plata"
)

# ==============================================================================
# Installation
# ==============================================================================
install(TARGETS plata
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ==============================================================================
# Finalization
# ==============================================================================
qt_finalize_executable(plata)
