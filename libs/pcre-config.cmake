# Copyright (c) 2012, Andre Caron (andre.l.caron@gmail.com)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

if(NOT pcre_FOUND)

  # Register library targets when found as part of a dependent project.
  # Since this project uses a find_package() directive to include this
  # file, don't recurse back into the CMakeLists file.

  # Custom PCRE library build options.
  add_definitions(
    -DPCRE_STATIC
  )
  set(PCRE_STATIC ON CACHE STRING "PCRE static")
  set(PCRE_BUILD_PCRE8 ON CACHE STRING "PCRE8")
  set(PCRE_BUILD_PCRE16 ON CACHE STRING "PCRE16")
  set(PCRE_BUILD_PCRECPP OFF CACHE STRING "PCRECPP")
  set(PCRE_SUPPORT_UTF ON CACHE STRING "PCRE UTF support")
  add_subdirectory(
    ${pcre_DIR}/pcre
    ${CMAKE_CURRENT_BINARY_DIR}/pcre
  )

  # Locate library headers.
  find_path(pcre_include_dir
    NAMES pcre.h
    PATHS ${CMAKE_CURRENT_BINARY_DIR}/pcre
  )
  message(STATUS "PCRE: '${pcre_include_dir}'.")

  # Export library targets.
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(pcre_libraries
      pcred pcre16d
      CACHE INTERNAL "PCRE library" FORCE
    )
  else()
    set(pcre_libraries
      pcre pcre16
      CACHE INTERNAL "PCRE library" FORCE
    )
  endif()

  # Usual "required" et. al. directive logic.
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    pcre DEFAULT_MSG
    pcre_include_dir
    pcre_libraries
  )

endif()
