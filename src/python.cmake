
option(VERA_USE_SYSTEM_PYTHON "Build python" ON)
mark_as_advanced(VERA_USE_SYSTEM_PYTHON)

if(VERA_USE_SYSTEM_PYTHON)
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  find_package(PythonInterp 2.0)
  find_package(PythonLibs 2.0 REQUIRED)
  include_directories(BEFORE SYSTEM ${PYTHON_INCLUDE_DIR})
  link_directories(${PYTHON_LIBRARY_DIR})
  # no target
  set(Python_TARGET)
else()
  include(ExternalProject)

  # a cmakeified version of python
  # prepare options for the extra modules
  set(opts)
  foreach(m
    ARRAY
    AUDIOOP
    BISECT
    BZ2
    CMATH
    CODECS_CN
    CODECS_HK
    CODECS_ISO2022
    CODECS_JP
    CODECS_KR
    CODECS_TW
    COLLECTIONS
    CPICKLE
    CRYPT
    CSTRINGIO
    CSV
    CTYPES_TEST
    DATETIME
    FCNTL
    FUNCTOOLS
    FUTURE_BUILTINS
    GRP
    HEAPQ
    HOTSHOT
    IO
    ITERTOOLS
    JSON
    LINUXAUDIODEV
    LOCALE
    LSPROF
    MATH
    MMAP
    MULTIBYTECODEC
    MULTIPROCESSING
    NIS
    OPERATOR
    OSSAUDIODEV
    PARSER
    RANDOM
    RESOURCE
    SELECT
    SOCKET
    SPWD
    STROP
    STRUCT
    SYSLOG
    TERMIOS
    TIME
    UNICODEDATA)
    list(APPEND opts -DENABLE_${m}:BOOL=ON)
    list(APPEND opts -DBUILTIN_${m}:BOOL=ON)
  endforeach()
  # disable modules that depends on extra libs
  foreach(m
    CURSES
    CURSES_PANEL
    READLINE
    ELEMENTTREE
    PYEXPAT
    HASHLIB
    SSL
    MD5
    SHA
    SHA
    SHA256
    SHA512
    SQLITE3
    TKINTER
    BINASCII
    ZLIB
    BSDDB
    DBM
    GDBM
    # these two ones fail because of undefined symbols
    SCPROXY
    TESTCAPI)
    list(APPEND opts -DENABLE_${m}:BOOL=OFF)
    list(APPEND opts -DBUILTIN_${m}:BOOL=OFF)
  endforeach()
  ExternalProject_Add(python
    URL https://github.com/glehmann/python-cmake-buildsystem/archive/official-python-src.zip
    URL_MD5 b3e045924c2506287ba568aed28ca719
    CMAKE_ARGS -Wno-dev
    CMAKE_CACHE_ARGS
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DBUILD_TESTING:BOOL=OFF
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_C_FLAGS:STRING=-w
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DUSE_SYSTEM_LIBRARIES:BOOL=ON
      ${opts}
    )
  ExternalProject_Get_Property(python INSTALL_DIR)
  ExternalProject_Get_Property(python BINARY_DIR)
  if(WIN32)
    set(PYTHON_LIBRARIES debug ${BINARY_DIR}/Debug/python2.7.lib
	  optimized ${BINARY_DIR}/Release/python2.7.lib)
  else()
    set(PYTHON_LIBRARIES ${INSTALL_DIR}/lib/libpython2.7.a)
  endif()
  include_directories(SYSTEM ${INSTALL_DIR}/include/python2.7)
  link_directories(${BINARY_DIR})
  set(Python_TARGET python)
endif()
