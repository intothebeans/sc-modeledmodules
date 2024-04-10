#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "iir::iir" for configuration "Debug"
set_property(TARGET iir::iir APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(iir::iir PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libiir.so.1.9.4"
  IMPORTED_SONAME_DEBUG "libiir.so.1"
  )

list(APPEND _cmake_import_check_targets iir::iir )
list(APPEND _cmake_import_check_files_for_iir::iir "${_IMPORT_PREFIX}/lib/libiir.so.1.9.4" )

# Import target "iir::iir_static" for configuration "Debug"
set_property(TARGET iir::iir_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(iir::iir_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libiir_static.a"
  )

list(APPEND _cmake_import_check_targets iir::iir_static )
list(APPEND _cmake_import_check_files_for_iir::iir_static "${_IMPORT_PREFIX}/lib/libiir_static.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
