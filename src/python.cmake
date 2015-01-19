
option(VERA_USE_SYSTEM_PYTHON "Build python" ON)
mark_as_advanced(VERA_USE_SYSTEM_PYTHON)

if(VERA_USE_SYSTEM_PYTHON)
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  find_package(PythonInterp 2.0)
  find_package(PythonLibs 2.0)
  if(WIN32 AND NOT PYTHONLIBS_FOUND)
    message(FATAL_ERROR "Could NOT find Python. Turn VERA_USE_SYSTEM_PYTHON to OFF to build it with vera++.")
  endif()
  include_directories(BEFORE SYSTEM ${PYTHON_INCLUDE_DIR})
  link_directories(${PYTHON_LIBRARY_DIR})
  # no target
  set(Python_TARGET)
else()
  include(ExternalProject)

  # a cmakeified version of python
  # prepare options for the extra modules
  set(opts)
  # foreach(m
  #   ARRAY
  #   AUDIOOP
  #   BISECT
  #   BZ2
  #   CMATH
  #   CODECS_CN
  #   CODECS_HK
  #   CODECS_ISO2022
  #   CODECS_JP
  #   CODECS_KR
  #   CODECS_TW
  #   COLLECTIONS
  #   CPICKLE
  #   CRYPT
  #   CSTRINGIO
  #   CSV
  #   CTYPES_TEST
  #   DATETIME
  #   FCNTL
  #   FUNCTOOLS
  #   FUTURE_BUILTINS
  #   GRP
  #   HEAPQ
  #   HOTSHOT
  #   IO
  #   ITERTOOLS
  #   JSON
  #   LINUXAUDIODEV
  #   LOCALE
  #   LSPROF
  #   MATH
  #   MMAP
  #   MULTIBYTECODEC
  #   MULTIPROCESSING
  #   NIS
  #   OPERATOR
  #   OSSAUDIODEV
  #   PARSER
  #   RANDOM
  #   RESOURCE
  #   SELECT
  #   SOCKET
  #   SPWD
  #   STROP
  #   STRUCT
  #   SYSLOG
  #   TERMIOS
  #   TIME
  #   UNICODEDATA)
  #   list(APPEND opts -DENABLE_${m}:BOOL=ON)
  #   list(APPEND opts -DBUILTIN_${m}:BOOL=ON)
  # endforeach()
  # disable modules that depends on extra libs
  # foreach(m
  #   CURSES
  #   CURSES_PANEL
  #   READLINE
  #   ELEMENTTREE
  #   PYEXPAT
  #   HASHLIB
  #   SSL
  #   MD5
  #   SHA
  #   SHA
  #   SHA256
  #   SHA512
  #   SQLITE3
  #   TKINTER
  #   BINASCII
  #   ZLIB
  #   BSDDB
  #   DBM
  #   GDBM
  #   # these two ones fail because of undefined symbols
  #   SCPROXY
  #   TESTCAPI)
  #   list(APPEND opts -DENABLE_${m}:BOOL=OFF)
  #   list(APPEND opts -DBUILTIN_${m}:BOOL=OFF)
  # endforeach()
  if(WIN32)
    set(cExtraFlags)
    set(cxxExtraFlags)
  else()
    set(cExtraFlags -w)
    set(cxxExtraFlags -w)
  endif()
  ExternalProject_Add(python
    URL https://github.com/verateam/python-cmake-buildsystem/archive/vera-1.3.0.zip
    URL_MD5 350b90184f266eda357d54677e5873b5
    CMAKE_ARGS -Wno-dev
    CMAKE_CACHE_ARGS
      -DBUILD_SHARED:BOOL=ON
      -DBUILD_STATIC:BOOL=OFF
      -DBUILD_TESTING:BOOL=OFF
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS} ${cxxExtraFlags}"
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      "-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS} ${cExtraFlags}"
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DUSE_SYSTEM_LIBRARIES:BOOL=ON
      ${opts}
    )
  ExternalProject_Get_Property(python INSTALL_DIR)
  ExternalProject_Get_Property(python BINARY_DIR)
  if(WIN32)
    # copy the runtime near vera++ executable so we can run the tests
    ExternalProject_Add_Step(python install_dll
      COMMAND ${CMAKE_COMMAND} -E copy
        ${INSTALL_DIR}/bin/python27.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/python27.dll
      DEPENDEES install)
    ExternalProject_Add_Step(python install_libs
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${INSTALL_DIR}/lib
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/lib
      DEPENDEES install)
    set(PYTHON_EXECUTABLE ${INSTALL_DIR}/bin/python.exe)
    set(PYTHON_LIBRARIES debug ${BINARY_DIR}/libs/Debug/python27.lib
          optimized ${BINARY_DIR}/libs/Release/python27.lib)
    set(PYTHON_INCLUDE_DIR  ${INSTALL_DIR}/include)
    list(APPEND CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "${INSTALL_DIR}/bin/python27.dll")
    install(DIRECTORY ${INSTALL_DIR}/lib DESTINATION bin)
  elseif(APPLE)
    message(WARNING "Python won't be installed. Setting VERA_USE_SYSTEM_PYTHON to OFF should only be used for testing purpose on this system.")
    set(PYTHON_EXECUTABLE ${INSTALL_DIR}/bin/python)
    set(PYTHON_LIBRARIES ${INSTALL_DIR}/lib/libpython2.7.dylib)
    set(PYTHON_INCLUDE_DIR  ${INSTALL_DIR}/include/python2.7)
  else()
    message(WARNING "Python won't be installed. Setting VERA_USE_SYSTEM_PYTHON to OFF should only be used for testing purpose on this system.")
    set(PYTHON_EXECUTABLE ${INSTALL_DIR}/bin/python)
    set(PYTHON_LIBRARIES ${INSTALL_DIR}/lib/libpython2.7.so)
    set(PYTHON_INCLUDE_DIR  ${INSTALL_DIR}/include/python2.7)
  endif()
  set(PYTHON_VERSION 2.7.3)
  include_directories(SYSTEM ${PYTHON_INCLUDE_DIR})
  link_directories(${BINARY_DIR})
  set(Python_TARGET python)
endif()

if(MSVC)
  add_definitions(-DHAVE_ROUND)
endif()
