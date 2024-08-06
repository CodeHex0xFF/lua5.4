#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <string.h>

// greet 方法
int l_personGreet(lua_State *L) {
  // self 是调用方法的表，位于堆栈顶部
  lua_getfield(L, 1, "name");             // 从 self 表中获取 name 字段
  const char *name = lua_tostring(L, -1); // 获取字符串值
  lua_pop(L, 1);                          // 弹出 name

  // 返回问候字符串
  lua_pushfstring(L, "Hello, my name is %s!", name);
  return 1; // 返回一个字符串
}

// 构造函数
int l_newPerson(lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  int age = luaL_checkinteger(L, 2);

  // 创建一个新的表作为对象
  lua_newtable(L);

  // 设置 name 字段
  lua_pushstring(L, name);
  lua_setfield(L, -2, "name");

  // 设置 age 字段
  lua_pushinteger(L, age);
  lua_setfield(L, -2, "age");

  // 设置元表
  luaL_getmetatable(L, "PersonMetaTable");
  lua_setmetatable(L, -2);

  return 1; // 返回新创建的对象
}

// 注册类和方法
int luaopen_test(lua_State *L) {
  // 创建元表
  luaL_newmetatable(L, "PersonMetaTable");

  // 设置 __index 元方法
  lua_pushvalue(L, -1); // 将元表自身压栈
  lua_setfield(L, -2, "__index");

  // 注册 greet 方法到元表
  lua_pushcfunction(L, l_personGreet);
  lua_setfield(L, -2, "greet");

  // 创建库表并注册构造函数
  lua_newtable(L);
  lua_pushcfunction(L, l_newPerson);
  lua_setfield(L, -2, "new");

  return 1; // 返回库表
}
