include(FindPackageHandleStandardArgs)

find_library(Libunistring_LIBRARY
    NAMES unistring
)

find_path(Libunistring_INCLUDE_PATH
    NAMES unictype.h
)

find_package_handle_standard_args(Libunistring
    REQUIRED_VARS Libunistring_LIBRARY Libunistring_INCLUDE_PATH
)

if(Libunistring_FOUND AND NOT TARGET Libunistring::Libunistring)
    add_library(Libunistring::Libunistring UNKNOWN IMPORTED)
    set_property(TARGET Libunistring::Libunistring PROPERTY IMPORTED_LOCATION "${Libunistring_LIBRARY}")
    set_property(TARGET Libunistring::Libunistring PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Libunistring_INCLUDE_PATH}")
endif()
