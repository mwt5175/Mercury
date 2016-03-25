/*****************************************************

	HgLuaThis.h
		Lua "this" table routines

*****************************************************/

#ifndef LUATHIS_H_INCLUDED
#define LUATHIS_H_INCLUDED

#include <lua/luainc.h>
#include "luaVirtualMachine.h"

namespace hg {

namespace script {

class HgLuaThis {

public:

	HgLuaThis (HgLuaVirtualMachine& vm, int iRef) : m_iOldRef (0), m_vm (vm)
	{
		lua_State *state = (lua_State *) vm;
		if (vm.ok ())
		{
			//! Save the old "this" table
			lua_getglobal (state, "this");
			m_iOldRef = luaL_ref (state, LUA_REGISTRYINDEX);

			//! Replace it with the new one
			lua_rawgeti(state, LUA_REGISTRYINDEX, iRef);
			lua_setglobal (state, "this");
		}
	}

	virtual ~HgLuaThis (void)
	{
		lua_State *state = (lua_State *) m_vm;
		if (m_iOldRef > 0 && m_vm.ok ())
		{
			//! Replace the old "this" table
			lua_rawgeti(state, LUA_REGISTRYINDEX, m_iOldRef);
			lua_setglobal (state, "this");
			luaL_unref (state, LUA_REGISTRYINDEX, m_iOldRef);
		}
	}


protected:
	int m_iOldRef;
	HgLuaVirtualMachine& m_vm;
};

}

}

#endif
