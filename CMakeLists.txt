CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
project("Lua5.4 Scripts")
FILE(GLOB SRC "./src/*.c")
FILE(GLOB SQLITE_MOD "./lsqlite3/lsqlite3.c")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
INCLUDE_DIRECTORIES("./src" "./lsqlite3")
ADD_LIBRARY(lua5.4 SHARED ${SRC})
ADD_LIBRARY(lsqlite3 SHARED ${SQLITE_MOD})
target_link_libraries(lua5.4 PUBLIC m dl readline pthread)
set_target_properties(lsqlite3 PROPERTIES PREFIX "")
TARGET_LINK_LIBRARIES(lsqlite3 PUBLIC m dl sqlite3 lua5.4)
ADD_EXECUTABLE(lua "./src/main/lua.c")
TARGET_LINK_LIBRARIES(lua PUBLIC dl m readline lua5.4)
ADD_DEFINITIONS(-DLUA_USE_LINUX
                -DLUA_USE_READLINE
                -DLUA_COMPAT_5_3
                -DLUA_ROOT="./")
