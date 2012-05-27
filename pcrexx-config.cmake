# Library configuration file used by dependent projects
# via find_package() built-in directive in "config" mode.

if(NOT DEFINED pcrexx_FOUND)

  # Locate library headers.
  find_path(pcrexx_include_dir
    NAMES pcre.hpp
    PATHS ${pcrexx_DIR}/code
  )

  # Export library targets.
  set(${PROJECT_NAME}_libraries
    pcrexx
    CACHE INTERNAL "pcrexx library" FORCE
  )

  # Usual "required" et. al. directive logic.
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    pcrexx DEFAULT_MSG
    pcrexx_include_dir
    ${PROJECT_NAME}_libraries
  )

  # Register library targets when found as part of a dependent project.
  # Since this project uses a find_package() directive to include this
  # file, don't recurse back into the CMakeLists file.
  if(NOT ${PROJECT_NAME} STREQUAL pcrexx)
    add_subdirectory(
      ${pcrexx_DIR}
      ${CMAKE_CURRENT_BINARY_DIR}/pcrexx
    )
  endif()
endif()
