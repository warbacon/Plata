cmake_minimum_required(VERSION 3.19)
project(Plata LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

if(WIN32)
    set(MUPDF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/mupdf/include")

    set(MUPDF_LIBRARY_DEBUG "${CMAKE_SOURCE_DIR}/mupdf/platform/win32/x64/Debug/libmupdf.lib")
    set(MUPDF_LIBRARY_RELEASE "${CMAKE_SOURCE_DIR}/mupdf/platform/win32/x64/Release/libmupdf.lib")

    set(MUPDF_LIBRARIES
        $<$<CONFIG:Debug>:${MUPDF_LIBRARY_DEBUG}>
        $<$<CONFIG:Release>:${MUPDF_LIBRARY_RELEASE}>
    )
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MUPDF REQUIRED mupdf)
else()
    message(FATAL_ERROR "NOT SUPPORTED")
endif()

qt_standard_project_setup()

qt_add_executable(plata
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/plata.cpp
    src/plata.h
    src/plata.ui
)

target_include_directories(plata PRIVATE ${MUPDF_INCLUDE_DIR})

target_link_libraries(plata PRIVATE
    Qt::Core
    Qt::Widgets
    ${MUPDF_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS plata
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET plata
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
