cmake_minimum_required(VERSION 3.28)
project(Plata VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# ==============================================================================
# Qt
# ==============================================================================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ==============================================================================
# MuPDF
# ==============================================================================
set(MUPDF_VERSION 1.26.11)
set(MUPDF_HASH 67798ec73ba8b70c45893e07497a2107a551f05a)

include(FetchContent)
FetchContent_Declare(
    mupdf
    URL https://casper.mupdf.com/downloads/archive/mupdf-${MUPDF_VERSION}-source.tar.gz
    URL_HASH SHA1=${MUPDF_HASH}
)
FetchContent_MakeAvailable(mupdf)

if(WIN32)
    set(MUPDF_WORKING_DIRECTORY ${mupdf_SOURCE_DIR}/platform/win32)
    set(MUPDF_BUILD_COMMAND
        msbuild mupdf.sln
        /p:Configuration=$<CONFIG>
        /p:Platform=x64
        /p:PlatformToolset=v143
        /t:libmupdf
        /nologo
        /v:minimal
    )
    set(MUPDF_LIBRARIES
        ${mupdf_SOURCE_DIR}/platform/win32/x64/$<CONFIG>/libmupdf.lib
    )
else()
    set(MUPDF_WORKING_DIRECTORY ${mupdf_SOURCE_DIR})
    set(MUPDF_BUILD_COMMAND make libs)
    set(MUPDF_LIBRARIES
        ${mupdf_SOURCE_DIR}/build/release/libmupdf.a
        ${mupdf_SOURCE_DIR}/build/release/libmupdf-third.a
    )
endif()

add_custom_target(build_mupdf
    COMMAND ${MUPDF_BUILD_COMMAND}
    WORKING_DIRECTORY ${MUPDF_WORKING_DIRECTORY}
    BYPRODUCTS ${MUPDF_LIBRARIES}
    COMMENT "Building MuPDF..."
)

add_library(MuPDF::MuPDF INTERFACE IMPORTED GLOBAL)
set_target_properties(MuPDF::MuPDF PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${mupdf_SOURCE_DIR}/include
    INTERFACE_LINK_LIBRARIES "${MUPDF_LIBRARIES}"
)
add_dependencies(MuPDF::MuPDF build_mupdf)

# ==============================================================================
# Main executable
# ==============================================================================
qt_add_executable(plata
    src/main.cpp
    src/pdfviewer.cpp
    src/pdfviewer.h
    src/pdfrenderer.cpp
    src/pdfrenderer.h
    src/pdfviewer.ui
    src/resources.qrc
)

target_link_libraries(plata PRIVATE
    Qt6::Widgets
    MuPDF::MuPDF
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
qt_generate_deploy_app_script(
    TARGET plata
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
