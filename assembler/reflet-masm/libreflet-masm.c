/*------------------------------------------------\
|This file let us call reflet-masm's main fonction|
|from an other program.                           |
\------------------------------------------------*/

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <reflet-pasm.h>
#include <stdbool.h>

//wrapper for mini_assembleFile
int lua_mini_assembleFile(lua_State* L){
    const char* fileIn = luaL_checkstring(L, -3);
    const char* fileOut = luaL_checkstring(L, -2);
    bool set_prefix = lua_toboolean(L, -1);
    mini_assembleFile(fileIn, fileOut, set_prefix);
    return 0;
}

int refletmasm(const char* fileIn,const char* fileOut){
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    //adding mini assembler function
    lua_pushcfunction(L, lua_mini_assembleFile);
    lua_setglobal(L, "assembleFile");

    //oppening files
    luaL_dofile(L, "/usr/share/reflet/lib.luac");
    luaL_dofile(L, "/usr/share/reflet/link.luac");
    luaL_dofile(L, "/usr/share/reflet/macro.luac");
    luaL_dofile(L, "/usr/share/reflet/readingCode.luac");
    luaL_dofile(L, "/usr/share/reflet/macro_assembler.luac");
    luaL_dofile(L, "/usr/local/share/reflet/lib.luac");
    luaL_dofile(L, "/usr/local/share/reflet/link.luac");
    luaL_dofile(L, "/usr/local/share/reflet/macro.luac");
    luaL_dofile(L, "/usr/local/share/reflet/readingCode.luac");
    luaL_dofile(L, "/usr/local/share/reflet/macro_assembler.luac");
    luaL_dofile(L, "lib.lua");
    luaL_dofile(L, "link.lua");
    luaL_dofile(L, "macro.lua");
    luaL_dofile(L, "readingCode.lua");
    luaL_dofile(L, "macro_assembler.lua");

    //loading main lua function
    lua_getglobal(L, "macro_assembler");

    //pushing arguments
    lua_createtable(L, 2, 0);
    lua_pushstring(L, fileIn);
    lua_seti(L, -2, 1);
    lua_pushstring(L, fileOut);
    lua_seti(L, -2, 2);

    lua_call(L,1,1);
    int ret = luaL_checkinteger(L, -1);
    lua_close(L);
    return ret;
}

