prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
includedir=${prefix}/@CMAKE_INSTALL_INCLUDEDIR@
libdir=${prefix}/@CMAKE_INSTALL_LIBDIR@

Name: donnell
Description: A simple C library for drawing graphics
Requires.Private: @REQUIRES_PRIVATE@
Version: 1.0
Cflags: -I${includedir}
Libs: -L${libdir} -ldonnell
Libs.Private: -lunistring
