cmake_minimum_required(VERSION 3.16)
project(libdonnell C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(CheckCSourceCompiles)
include(CMakePushCheckState)
include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build libdonnell as a shared library" TRUE)
option(LIBDONNELL_EXAMPLES "Build libdonnell examples" TRUE)

find_package(PkgConfig REQUIRED)

set(libdonnell_PKGCONFIG_DEPS fribidi fontconfig freetype2 libpng)
pkg_check_modules(PC_LIBDONNELL_DEPS REQUIRED IMPORTED_TARGET fribidi fontconfig freetype2 libpng)
find_package(Libunistring REQUIRED)

cmake_push_check_state(RESET)
set(src_roundf_test "#include <math.h>\nint main(int argc){return roundf(0.4f * (float)argc);}\n")
check_c_source_compiles("${src_roundf_test}" LIBC_HAS_ROUNDF)
if(NOT LIBC_HAS_ROUNDF)
    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_c_source_compiles("${src_roundf_test}" LIBM_HAS_ROUNDF)
endif()
cmake_pop_check_state(RESET)

add_library(libdonnell
    src/bidi.c
    src/donnell.c
    src/fontconfig.c
    src/freetype.c
    src/graphicsprimitives.c
    src/guiprimitives.c
    src/harfbuzz.c
    src/imagebuf.c
    src/misc.c
    src/pixel.c
    src/stockimages.c
    src/svg.c
    src/textrenderer.c
    src/textutils.c
)
add_library(libdonnell::libdonnell ALIAS libdonnell)
set_property(TARGET libdonnell PROPERTY OUTPUT_NAME "donnell")
set_property(TARGET libdonnell PROPERTY C_VISIBILITY_PRESET "hidden")
target_include_directories(libdonnell PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assets>")
target_include_directories(libdonnell PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_include_directories(libdonnell PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(libdonnell PRIVATE PkgConfig::PC_LIBDONNELL_DEPS Libunistring::Libunistring ${CMAKE_DL_LIBS})
if(LIBM_HAS_ROUNDF AND NOT LIBC_HAS_ROUNDF)
    target_link_libraries(libdonnell PRIVATE m)
endif()
target_link_options(libdonnell PRIVATE "-Wl,--no-undefined")

string(JOIN " " REQUIRES_PRIVATE ${libdonnell_PKGCONFIG_DEPS})
configure_file(cmake/donnell.pc.cmake donnell.pc @ONLY)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/donnell.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/donnell.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
install(TARGETS libdonnell
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

if(LIBDONNELL_EXAMPLES)
    add_executable(libdonnell-example examples/example.c)
    target_link_libraries(libdonnell-example PRIVATE libdonnell::libdonnell)
    add_executable(libdonnell-example-titlebar examples/example.c)
    target_link_libraries(libdonnell-example-titlebar PRIVATE libdonnell::libdonnell)
    find_package(X11)
    if(X11_FOUND)
        add_executable(libdonnell-example-x11 examples/example-x11.c)
        target_link_libraries(libdonnell-example-x11 PRIVATE libdonnell::libdonnell X11::X11)
        add_executable(libdonnell-example-x11-titlebar examples/example-titlebar-x11.c)
        target_link_libraries(libdonnell-example-x11-titlebar PRIVATE libdonnell::libdonnell X11::X11)
    endif()
endif()
