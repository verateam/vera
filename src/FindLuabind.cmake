##########################################################################################
# note: this is from http://old.nabble.com/Cmake-file-for-finding-Luabind-p28615850.html #
# created by Nigel Atkinson-2                                                            #
# (changed from original FindLua51.cmake provided with CMake (www.cmake.org), which was  #
#  provided under the licence below (original licence) )                                 #
##########################################################################################

#===================================================================================
# the original file was provided under the following licence:
# (see http://cmake.org/cmake/project/license.html)

# CMake - Cross Platform Makefile Generator
# Copyright 2000-2009 Kitware, Inc., Insight Software Consortium
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium, nor the names of their contributors may be used to endorse or promote products derived from this software without specific prior written  permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#====================================================================================

# Locate Luabind library
# This module defines
#  LUABIND_FOUND, if false, do not try to link to Luabind
#  LUABIND_LIBRARIES
#  LUABIND_INCLUDE_DIR, where to find luabind.hpp
#
# Note that the expected include convention is
#  #include <luabind/luabind.hpp>
# and not
#  #include <luabind.hpp>

IF( NOT LUABIND_FIND_QUIETLY )
    MESSAGE(STATUS "Looking for Luabind...")
ENDIF()

FIND_PATH(LUABIND_INCLUDE_DIR luabind/luabind.hpp
  HINTS
  $ENV{LUABIND_DIR}
  PATH_SUFFIXES luabind include/luabind include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local # DarwinPorts
  /opt
)

FIND_LIBRARY(LUABIND_LIBRARY
  NAMES luabind
  HINTS
  $ENV{LUABIND_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local
  /opt
)

FIND_LIBRARY(LUABIND_LIBRARY_DBG
  NAMES luabindd
  HINTS
  $ENV{LUABIND_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local
  /opt
)

IF(LUABIND_LIBRARY)
    SET( LUABIND_LIBRARIES "${LUABIND_LIBRARY}" CACHE STRING "Luabind Libraries")
ENDIF(LUABIND_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LUABIND_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Luabind  DEFAULT_MSG  LUABIND_LIBRARIES LUABIND_INCLUDE_DIR)

IF( NOT LUABIND_FIND_QUIETLY )
    IF( LUABIND_FOUND )
        MESSAGE(STATUS "Found Luabind: ${LUABIND_LIBRARY}" )
    ENDIF()
    IF( LUABIND_LIBRARY_DBG )
        MESSAGE(STATUS "Luabind debug library availible: ${LUABIND_LIBRARY_DBG}")
    ENDIF()
ENDIF()

MARK_AS_ADVANCED(LUABIND_INCLUDE_DIR LUABIND_LIBRARIES LUABIND_LIBRARY LUABIND_LIBRARY_DBG) 
