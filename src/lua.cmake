
option(VERA_USE_SYSTEM_LUA "Build lua and luabind" ON)
mark_as_advanced(VERA_USE_SYSTEM_LUA)

if(VERA_USE_SYSTEM_LUA)
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  find_package(Lua51 REQUIRED)
  find_package(Luabind REQUIRED)
  include_directories(BEFORE SYSTEM ${LUABIND_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
  link_directories(${LUABIND_LIBRARY_DIR} ${LUA_LIBRARY_DIR})
  # no target
  set(Lua_TARGET)
else()
  include(ExternalProject)

  # a cmakeified version of lua
  ExternalProject_Add(lua
    URL https://github.com/LuaDist/lua/archive/5.1.5.tar.gz
    URL_MD5 5bb22e7b5368aea321080f659df0bf5a
    CMAKE_ARGS -Wno-dev
    CMAKE_CACHE_ARGS
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_C_FLAGS:STRING=-w
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    )
  ExternalProject_Get_Property(lua INSTALL_DIR)
  ExternalProject_Get_Property(lua BINARY_DIR)
  if(WIN32)
    set(LUA_LIBRARIES debug ${BINARY_DIR}/Debug/liblua_static.lib
	  optimized ${BINARY_DIR}/Release/liblua_static.lib)
  else()
    set(LUA_LIBRARIES ${BINARY_DIR}/libliblua_static.a)
  endif()
  set(lua_include_dir ${INSTALL_DIR}/include)
  include_directories(SYSTEM ${lua_include_dir})
  link_directories(${BINARY_DIR})
  set(Lua_TARGET lua)

  # luabind seems unmaintained by its main developer. This version supports
  # lua 5.2
  if(TARGET boost)
    ExternalProject_Get_Property(boost SOURCE_DIR)
    set(boostDir ${SOURCE_DIR})
  else()
    # guess the boost dir based on the include dir
    set(boostDir ${Boost_INCLUDE_DIR}/..)
  endif()
  ExternalProject_Add(luabind
    URL https://github.com/glehmann/luabind/archive/luabind-for-vera.zip
    URL_MD5 8d1fd26a66da098283969eaf6253ebe3
    CMAKE_CACHE_ARGS
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DBUILD_TESTING:BOOL=OFF
      -DCMAKE_CXX_FLAGS:STRING=-w
      -DLUA_INCLUDE_DIR:PATH=${lua_include_dir}
      -DLUA_LIBRARIES:PATH=${LUA_LIBRARIES}
      -DLUA_LIBRARY:PATH=${LUA_LIBRARIES}
      -DBOOST_ROOT:PATH=${boostDir}
    INSTALL_COMMAND "")
  add_dependencies(luabind lua)
  if(TARGET boost)
    add_dependencies(luabind boost)
  endif()
  ExternalProject_Get_Property(luabind SOURCE_DIR)
  ExternalProject_Get_Property(luabind BINARY_DIR)
  if(WIN32)
    set(LUA_LIBRARIES debug ${BINARY_DIR}/src/Debug/luabind.lib
	  optimized ${BINARY_DIR}/src/Release/luabind.lib)
  else()
    set(LUABIND_LIBRARY ${BINARY_DIR}/src/libluabind.a)
  endif()
  include_directories(SYSTEM ${SOURCE_DIR})
  link_directories(${BINARY_DIR}/src)
  set(Luabind_TARGET luabind)
endif()
