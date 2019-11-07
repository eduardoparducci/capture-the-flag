#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FreeGLUT::freeglut" for configuration ""
set_property(TARGET FreeGLUT::freeglut APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FreeGLUT::freeglut PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libglut.so.3.11.0"
  IMPORTED_SONAME_NOCONFIG "libglut.so.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS FreeGLUT::freeglut )
list(APPEND _IMPORT_CHECK_FILES_FOR_FreeGLUT::freeglut "${_IMPORT_PREFIX}/lib/libglut.so.3.11.0" )

# Import target "FreeGLUT::freeglut_static" for configuration ""
set_property(TARGET FreeGLUT::freeglut_static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FreeGLUT::freeglut_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libglut.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS FreeGLUT::freeglut_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_FreeGLUT::freeglut_static "${_IMPORT_PREFIX}/lib/libglut.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
