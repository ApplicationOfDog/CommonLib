// luaTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

extern "C"
{
#include "lua.h"  
#include "lualib.h"  
#include "lauxlib.h"  
#include "luaconf.h"  
};

int main(int argc, char* argv[])  
{  
	lua_State* L = luaL_newstate();  
	luaL_openlibs(L);  
	luaL_dofile(L, "C:/Documents and Settings/fxx/桌面/lua/debug/test.lua");  
	lua_close(L);  

	return 0;  
}  