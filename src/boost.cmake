
if(WIN32)
  option(VERA_USE_SYSTEM_BOOST "Use system boost" ON)
else()
  option(VERA_USE_SYSTEM_BOOST "Build boost" OFF)
endif()
mark_as_advanced(VERA_USE_SYSTEM_BOOST)

set(boostLibs filesystem system program_options regex wave)
set(pythonLib "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
if(VERA_PYTHON)
  # Note that Boost Python components require a Python version
  # suffix (Boost 1.67 and later), e.g. python36 or python27 for
  # the versions built against Python 3.6 and 2.7, respectively.
  # This also applies to additional components using Python
  # including mpi_python and numpy. Earlier Boost releases may use
  # distribution-specific suffixes such as 2, 3 or 2.7. These may also
  # be used as suffixes, but note that they are not portable.
  #
  # from https://cmake.org/cmake/help/latest/module/FindBoost.html
  list(APPEND boostLibs ${pythonLib})
endif()

if(VERA_USE_SYSTEM_BOOST)
  if(WIN32)
    # use boost static libs to avoid LNK2019 errors
    # feel free to contribute a better fix!
    set(Boost_USE_STATIC_LIBS ON)
    add_definitions(-DBOOST_PYTHON_STATIC_LIB)
  else()
    # expose the Boost_USE_STATIC_LIBS option to ease the manual creation of
    # packages with cpack
    option(Boost_USE_STATIC_LIBS "Use Boost static libraries" OFF)
  endif()
  find_package(Boost COMPONENTS ${boostLibs} REQUIRED)
  include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
  # hide Boost_DIR option that doesn't seem to be set by FindBoost
  mark_as_advanced(Boost_DIR)
  # no target
  set(Boost_TARGET)
else()
  include(ExternalProject)

  set(BOOST_MIRROR boostorg.jfrog.io CACHE STRING
    "Host used to download boost. Use it to force a specific mirror.")
  mark_as_advanced(BOOST_MIRROR)

  set(Boost_VERSION 1.77.0)
  set(Boost_URL "https://${BOOST_MIRROR}/artifactory/main/release/1.77.0/source/boost_1_77_0.tar.bz2")
  set(Boost_URL_HASH "SHA256=fc9f85fc030e233142908241af7a846e60630aa7388de9a5fafb1f3a26840854")

  string(REPLACE ${pythonLib} "python" boostLibs "${boostLibs}")
  string(REPLACE ";" "," boostLibsComma "${boostLibs}")
  string(REPLACE ";" " --with-" WITH_LIBS "${boostLibs}")
  set(WITH_LIBS "--with-${WITH_LIBS}")
  if(WIN32)
    set(variant "debug,release")
    set(bootstrap bootstrap.bat)
    string(REGEX REPLACE "Visual Studio ([0-9]+).*" "\\1" msvcver ${CMAKE_GENERATOR})
    set(TOOLSET msvc)
    set(cExtraFlags -DHAVE_ROUND)
    set(cxxExtraFlags -DHAVE_ROUND)
  else()
    if(APPLE)
      set(TOOLSET darwin)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      set(TOOLSET clang)
    else()
      set(TOOLSET gcc)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(variant debug)
    else()
      set(variant release)
    endif()
    set(bootstrap bootstrap.sh)
    set(cExtraFlags -w)
    set(cxxExtraFlags -w)
  endif()
  # generate the project-config.jam
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/project-config.jam.in ${CMAKE_CURRENT_BINARY_DIR}/project-config.jam @ONLY)
  if(VERA_PYTHON)
    set(b2ExtraFlags include=${PYTHON_INCLUDE_DIR})
    string(REPLACE ",python" "" boostLibsComma "${boostLibsComma}")
  else()
    set(b2ExtraFlags)
  endif()

  set(Boost_LIBRARIES)
  foreach(l ${boostLibs})
    # Starting with Boost 1.67, Boost Python's libary includes a version suffix.
    if(NOT (Boost_VERSION VERSION_LESS 1.67))
      if (${l} STREQUAL "python")
        set(l ${pythonLib})
      endif()
    endif()
    if(WIN32)
      list(APPEND Boost_LIBRARIES debug <SOURCE_DIR>/stage/lib/libboost_${l}-vc${msvcver}0-mt-gd-1_56.lib)
      list(APPEND Boost_LIBRARIES optimized <SOURCE_DIR>/stage/lib/libboost_${l}-vc${msvcver}0-mt-1_56.lib)
    else()
      list(APPEND Boost_LIBRARIES <SOURCE_DIR>/stage/lib/libboost_${l}.a)
    endif()
  endforeach()

  ExternalProject_Add(boost
    URL ${Boost_URL}
    URL_HASH ${Boost_URL_HASH}
    CONFIGURE_COMMAND ./${bootstrap} --with-libraries=${boostLibsComma} --with-toolset=${TOOLSET}
    BUILD_COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/project-config.jam
      <SOURCE_DIR>/project-config.jam
    INSTALL_COMMAND ./b2
      threading=multi
      link=static
      variant=${variant}
      warnings=off
      "cxxflags=-DBOOST_WAVE_SUPPORT_MS_EXTENSIONS=1 ${CMAKE_CXX_FLAGS} ${cxxExtraFlags}"
      "cflags=-DMAKE_SURE_CFLAGS_IS_NOT_EMPTY ${CMAKE_C_FLAGS} ${cExtraFlags}"
      -s NO_BZIP2=1
      ${b2ExtraFlags}
    BUILD_IN_SOURCE ON
    BUILD_BYPRODUCTS ${Boost_LIBRARIES})
  if(TARGET python)
    add_dependencies(boost python)
  endif()
   _ep_replace_location_tags(boost Boost_LIBRARIES)
  ExternalProject_Get_Property(boost SOURCE_DIR)
  include_directories(SYSTEM ${SOURCE_DIR})
  link_directories(${SOURCE_DIR}/stage/lib)
  add_definitions(-DBOOST_PYTHON_STATIC_LIB)
  set(Boost_TARGET boost)
endif()
