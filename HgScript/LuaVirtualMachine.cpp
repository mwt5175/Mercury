/*****************************************************

	luaVirtualMachine.cpp
		Lua Virtual Machine

*****************************************************/

#include <lua/luainc.h>
#include "luaVirtualMachine.h"

#include <string>
#include <stdio.h>

namespace hg {

namespace script {

static int printMessage (lua_State *lua)
{
	if (lua_isstring (lua, 1))
	{
		const char *msg = lua_tostring (lua, 1);

		// get caller
		lua_Debug ar;
		memset (&ar, 0, sizeof(ar));
		lua_getstack (lua, 1, &ar);
		lua_getinfo (lua, "Snl", &ar);

		// debug output
		const char *str = ar.source;
		printf ("script: %s -- at %s(%d)\n", msg, str, ar.currentline);
		return 0;
	} else {
		printf ("Lua Error: Trace invoked with non-string argument\n");
		return -1;
	}
}

HgLuaVirtualMachine::HgLuaVirtualMachine () : vmState (NULL)
{
	vmIsOk = false;
	
	//! Open Lua
	vmState = lua_open ();

	if (vmState) 
	{
		vmIsOk = true;

		//! Load util libs into lua
		luaopen_base (vmState);
		luaopen_table (vmState);
		luaopen_string (vmState);
		luaopen_math (vmState);
		luaopen_debug (vmState);
		luaopen_io (vmState);
		luaopen_loadlib (vmState);

		//! setup global printing
		lua_pushcclosure (vmState, printMessage, 0);
		lua_setglobal (vmState, "trace");

		//! Setup the panic routine (may not be called in protected mode)
		lua_atpanic (vmState, (lua_CFunction) HgLuaVirtualMachine::panic);
	}
}

//! Close lua
HgLuaVirtualMachine::~HgLuaVirtualMachine ()
{
	if (vmState)
	{
		lua_close (vmState);
		/*vmState = NULL;
		vmIsOk = false;*/
	}
}

// may not get called in protected mode
void HgLuaVirtualMachine::panic (lua_State *lua)
{
	printf ("Lua panic\n");
	for(;;);				//infinite loop for now
}

//! Run a file (Lua method)
/*bool luaVirtualMachine::RunFile (const char *strFilename)
{
	int errorNum = 0;

	if ((errorNum = luaL_loadfile (vmState, strFilename)) == 0)
	{
		// Call
		if ((errorNum = lua_pcall (vmState, 0, LUA_MULTRET, 0)) == 0)
		{
			return true;
		}
	}

	ErrorRun (errorNum);

	return false;
}*/

//! Run a file (mercury file manager)
bool HgLuaVirtualMachine::runFile (std::string pkFileName)
{
#if 0
	char* buffer = 0;
	int fileLen = 0;

	fileManager* fileMgr = kernel::get()->getFileManager ();
	if (!fileMgr)
		return 0;

	file* pkFile = fileMgr->open (pkFileName);
	if (!pkFile)
		return 0;

	if (!pkFile->open (pkFileName))
		return 0;

	fileLen = pkFile->getSize ();
	buffer = new char [ fileLen + 1 ];
	pkFile->read (buffer, fileLen);

	fileMgr->close (pkFile);

	//lua_pushfstring(vmState, "@%s", pkFileName.c_str ());

	if (runBuffer ((const char *)buffer, fileLen, pkFileName))
		return true;
#endif
	return false;
}

//! Run a buffer
bool HgLuaVirtualMachine::runBuffer (const char *pbBuffer, size_t szLen, std::string name )
{
	if (name.c_str() == NULL)
		name = "Temp";

	int errorNum = 0;

	if ((errorNum = luaL_loadbuffer (vmState, (const char *) pbBuffer, szLen, name.c_str())) == 0)
	{
		// Call
		if ((errorNum = lua_pcall (vmState, 0, LUA_MULTRET, 0)) == 0)
			return true;
	}


	luaError (errorNum);

	return false;   

}

//! Call a Lua function
bool HgLuaVirtualMachine::callFunction (int nArgs, int nReturns)
{   
	if (lua_isfunction (vmState, -nArgs-1))
	{
		int errorNum = 0;
		/*errorNum = lua_pcall (m_pState, nArgs, nReturns, 0);*/

		if ((errorNum = lua_pcall (vmState, nArgs, nReturns, 0)) == 0)
		{
			return true;
		}
		luaError (errorNum);
	}

	return false;
}

//! Lua Errors
void HgLuaVirtualMachine::luaError (int iErrorCode)
{
	switch (iErrorCode)
	{
	case LUA_ERRRUN:
		printf ("LUA_ERRRUN\n");
		break;
	case LUA_ERRMEM:
		printf ("LUA_ERRMEM\n");
		break;
	case LUA_ERRERR:
		printf ("LUA_ERRERR\n");
		break;
	}

	printf ("Error: %s\n", lua_tostring ((lua_State *) vmState, -1));
}

}

}
