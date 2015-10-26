###############################################################################
# Find DuraarkRDF
#
# This sets the following variables:
# DURAARKRDF_FOUND - True if DuraarkRDF was found.
# DURAARKRDF_INCLUDE_DIRS - Directories containing the DuraarkRDF include files.
# DURAARKRDF_LIBRARY_DIRS - Directories containing the DuraarkRDF library.
# DURAARKRDF_LIBRARIES - DuraarkRDF library files.

if(WIN32)
    find_path(DURAARKRDF_INCLUDE_DIR duraark_rdf PATHS "/usr/include" "/usr/local/include" "/usr/x86_64-w64-mingw32/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)

    find_library(DURAARKRDF_LIBRARY_PATH duraark_rdf PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib" NO_DEFAULT_PATHS)

    if(EXISTS ${DURAARKRDF_LIBRARY_PATH})
        get_filename_component(DURAARKRDF_LIBRARY ${DURAARKRDF_LIBRARY_PATH} NAME)
        find_path(DURAARKRDF_LIBRARY_DIR ${DURAARKRDF_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib" NO_DEFAULT_PATHS)
    endif()
else(WIN32)
    find_path(DURAARKRDF_INCLUDE_DIR duraark_rdf PATHS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)
    find_library(DURAARKRDF_LIBRARY_PATH duraark_rdf PATHS "/usr/lib" "/usr/local/lib" NO_DEFAULT_PATHS)

    if(EXISTS ${DURAARKRDF_LIBRARY_PATH})
        get_filename_component(DURAARKRDF_LIBRARY ${DURAARKRDF_LIBRARY_PATH} NAME)
        find_path(DURAARKRDF_LIBRARY_DIR ${DURAARKRDF_LIBRARY} PATHS "/usr/lib" "/usr/local/lib" NO_DEFAULT_PATHS)
    endif()
endif(WIN32)

set(DURAARKRDF_INCLUDE_DIRS ${DURAARKRDF_INCLUDE_DIR})
set(DURAARKRDF_LIBRARY_DIRS ${DURAARKRDF_LIBRARY_DIR})
set(DURAARKRDF_LIBRARIES ${DURAARKRDF_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DuraarkRDF DEFAULT_MSG DURAARKRDF_INCLUDE_DIR DURAARKRDF_LIBRARY DURAARKRDF_LIBRARY_DIR)

mark_as_advanced(DURAARKRDF_INCLUDE_DIR)
mark_as_advanced(DURAARKRDF_LIBRARY_DIR)
mark_as_advanced(DURAARKRDF_LIBRARY)
mark_as_advanced(DURAARKRDF_LIBRARY_PATH)
