#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Alembic::Alembic" for configuration "Debug"
set_property(TARGET Alembic::Alembic APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Alembic::Alembic PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/Alembic.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/Alembic.dll"
  )

list(APPEND _cmake_import_check_targets Alembic::Alembic )
list(APPEND _cmake_import_check_files_for_Alembic::Alembic "${_IMPORT_PREFIX}/debug/lib/Alembic.lib" "${_IMPORT_PREFIX}/debug/bin/Alembic.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
