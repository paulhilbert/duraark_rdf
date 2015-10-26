###############################################################################
# Find Raptor
#
# This sets the following variables:
# RAPTOR_FOUND - True if Raptor was found.
# RAPTOR_INCLUDE_DIRS - Directories containing the Raptor include files.
# RAPTOR_LIBRARY_DIRS - Directories containing the Raptor library.
# RAPTOR_LIBRARIES - Raptor library files.

if(WIN32)
    find_path(RAPTOR_INCLUDE_DIR raptor2 PATHS "/usr/include" "/usr/local/include" "/usr/x86_64-w64-mingw32/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)

    find_library(RAPTOR_LIBRARY_PATH raptor2 PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib" NO_DEFAULT_PATHS)

    if(EXISTS ${RAPTOR_LIBRARY_PATH})
        get_filename_component(RAPTOR_LIBRARY ${RAPTOR_LIBRARY_PATH} NAME)
        find_path(RAPTOR_LIBRARY_DIR ${RAPTOR_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib" NO_DEFAULT_PATHS)
    endif()
else(WIN32)
    find_path(RAPTOR_INCLUDE_DIR raptor2 PATHS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)
    find_library(RAPTOR_LIBRARY_PATH raptor2 PATHS "/usr/lib" "/usr/local/lib" NO_DEFAULT_PATHS)

    if(EXISTS ${RAPTOR_LIBRARY_PATH})
        get_filename_component(RAPTOR_LIBRARY ${RAPTOR_LIBRARY_PATH} NAME)
        find_path(RAPTOR_LIBRARY_DIR ${RAPTOR_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" NO_DEFAULT_PATHS)
    endif()
endif(WIN32)

set(RAPTOR_INCLUDE_DIRS ${RAPTOR_INCLUDE_DIR})
set(RAPTOR_LIBRARY_DIRS ${RAPTOR_LIBRARY_DIR})
set(RAPTOR_LIBRARIES ${RAPTOR_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Raptor DEFAULT_MSG RAPTOR_INCLUDE_DIR RAPTOR_LIBRARY RAPTOR_LIBRARY_DIR)

mark_as_advanced(RAPTOR_INCLUDE_DIR)
mark_as_advanced(RAPTOR_LIBRARY_DIR)
mark_as_advanced(RAPTOR_LIBRARY)
mark_as_advanced(RAPTOR_LIBRARY_PATH)
