CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
project("Lua5.4 Scripts")

FILE(GLOB SRC "./src/*.c")
FILE(GLOB SQLITE_MOD "./lsqlite3/lsqlite3.c")
FILE(GLOB C_PERIPHERY "./periphery/c-periphery/src/*.c")
FILE(GLOB LUA_PERIPHERY "./periphery/lua-periphery/src/*.c")
FILE(GLOB LUA_SOCKET "./luasocket/src/*.c")
FILE(GLOB TEST_SRC "./test/*.c")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(LUA_SCR_SRC "${CMAKE_SOURCE_DIR}/luasocket/src")
SET(LUA_SCR_DST "${CMAKE_BINARY_DIR}/share/lua/5.4")
SET(LUA_LIB_DST "${CMAKE_BINARY_DIR}/lib/lua/5.4")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LUA_LIB_DST})

INCLUDE_DIRECTORIES("./src" "./lsqlite3" "./periphery" "./periphery/lua-periphery/src" "./luasocket/src")

ADD_LIBRARY(c_periphery SHARED ${C_PERIPHERY})
ADD_LIBRARY(lua5.4 SHARED ${SRC})
ADD_LIBRARY(lsqlite3 SHARED ${SQLITE_MOD})
ADD_LIBRARY(periphery SHARED ${LUA_PERIPHERY})
ADD_LIBRARY(socket SHARED ${LUA_SOCKET})
ADD_LIBRARY(test SHARED ${TEST_SRC})

set_target_properties(socket PROPERTIES PREFIX "")
set_target_properties(periphery PROPERTIES PREFIX "")
set_target_properties(lsqlite3 PROPERTIES PREFIX "")
set_target_properties(test PROPERTIES PREFIX "")

target_link_libraries(lua5.4 PUBLIC m dl readline pthread)
target_link_libraries(periphery PUBLIC c_periphery lua5.4 m dl)
target_link_libraries(socket PUBLIC c_periphery lua5.4 m dl)

TARGET_LINK_LIBRARIES(lsqlite3 PUBLIC m dl sqlite3 lua5.4)
ADD_EXECUTABLE(lua "./src/main/lua.c")
TARGET_LINK_LIBRARIES(lua PUBLIC dl m readline lua5.4)
ADD_DEFINITIONS(-DLUA_USE_LINUX
                -DLUA_USE_READLINE
                -DLUA_COMPAT_5_3
                -DLUA_ROOT="./")



FILE(GLOB LUA_SCR_DST_FILES "${LUA_SCR_SRC}/*.lua")

FILE(MAKE_DIRECTORY ${LUA_SCR_DST})

add_custom_target(copy_scr_files ALL DEPENDS ${LUA_SCR_DST_FILES} ${LUA_LIB_DST_FILES})

foreach(file ${LUA_SCR_DST_FILES})
                add_custom_command(TARGET copy_scr_files PRE_BUILD COMMAND cp ${file} ${LUA_SCR_DST})
endforeach()
