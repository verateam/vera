
if(WIN32)
  option(VERA_USE_SYSTEM_BOOST "Use system boost" ON)
else()
  option(VERA_USE_SYSTEM_BOOST "Build boost" OFF)
endif()
mark_as_advanced(VERA_USE_SYSTEM_BOOST)

set(boostLibs filesystem system program_options regex wave)
if(VERA_PYTHON)
  list(APPEND boostLibs python)
endif()

if(VERA_USE_SYSTEM_BOOST)
  if(WIN32)
    # use boost static libs to avoid LNK2019 errors
    # feel free to contribute a better fix!
    set(Boost_USE_STATIC_LIBS ON)
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

  string(REPLACE boostLibsComma ";" "," ${boostLibs})
  set(opts -j4 threading=multi link=static "cxxflags=-DBOOST_WAVE_SUPPORT_MS_EXTENSIONS=1 -w" cflags=-w -s NO_BZIP2=1)
  ExternalProject_Add(boost
    URL http://downloads.sourceforge.net/project/boost/boost/1.57.0/boost_1_57_0.tar.bz2
    URL_MD5 1be49befbdd9a5ce9def2983ba3e7b76
    CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=${boostLibsComma} --prefix=<INSTALL_DIR>
    BUILD_COMMAND ./b2 ${opts}
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ./b2 ${opts} install)
  ExternalProject_Get_Property(boost INSTALL_DIR)
  set(Boost_LIBRARIES)
  foreach(l ${boostLibs})
    list(APPEND Boost_LIBRARIES ${INSTALL_DIR}/lib/libboost_${l}.a)
  endforeach()
  include_directories(SYSTEM ${INSTALL_DIR}/include)
  link_directories(${INSTALL_DIR}/lib)
  set(Boost_TARGET boost)
endif()
