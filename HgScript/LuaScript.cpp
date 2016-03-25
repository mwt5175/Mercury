/*****************************************************

	luaScript.cpp
		Lua script

*****************************************************/

#include <lua/luainc.h>
#include "luaScript.h"
#include "luaThis.h"
#include "LuaVirtualMachine.h"

namespace hg {

namespace script {

//! Restore the lua stack
class HgLuaRestoreStack
{
public:
	//! Get current top
	HgLuaRestoreStack (HgLuaVirtualMachine& vm) : State (NULL)
	{
		State = (lua_State *) vm;
		if (vm.ok ())
		{
			Top = lua_gettop (State);
		}
	}

	//! Restore top
	virtual ~HgLuaRestoreStack (void)
	{
		lua_settop (State, Top);
	}

protected:
	lua_State *State;
	int Top;
};


//! Call a script funtion
void HgLuaScript::call (const char *strFuncName, int ret, int lnumArgs, ...) {
    va_list listPointer;
    va_start(listPointer, lnumArgs);

	selectScriptFunction (strFuncName);
	for (int i = 0; i < lnumArgs; i++)
		addArg (va_arg(listPointer, int));
    va_end(listPointer);	
	run (ret);
}

//! Lua call back routine
static int callback (lua_State *lua)
{
	// Locate the psudo-index for the function number
	int NumberIdx = lua_upvalueindex (1);
	int RetsOnStack = 0;

	bool Success = false;

	// Check for the "this" table
	if (lua_istable (lua, 1))
	{
		// Found the "this" table. The object pointer is at the index 0
		lua_rawgeti (lua, 1, 0);
      
		if (lua_islightuserdata (lua, -1))
		{
			// Found the pointer, need to cast it
			HgLuaScript *pThis = (HgLuaScript *) lua_touserdata (lua, -1);

			// Get the method index
			int methodIdx = (int) lua_tonumber (lua, NumberIdx);

			// Check that the method is correct index
			if (methodIdx > pThis->methods ())
				return -1;

			// Reformat the stack so our parameters are correct
			// Clean up the "this" table
			lua_remove (lua, 1);
			// Clean up the pThis pointer
			lua_remove (lua, -1);

			// Call the class
			RetsOnStack = pThis->scriptCalling (pThis->getvm (), methodIdx);

			Success = true;
		}
	}

	if (Success == false)
	{
		lua_pushstring (lua, "LuaCallback -> Failed to call the class function");
		lua_error (lua);
	}

	// Number of return variables
	return RetsOnStack;
}

//! Constructor
HgLuaScript::HgLuaScript (HgLuaVirtualMachine& vm)
 : vm (vm), numMethods (0), thisRef (0), numArgs (0)
{
	if (vm.ok ())
	{
		lua_State *state = (lua_State*) vm;

		// Create a reference to the "this" table. Each reference is unique
		lua_newtable (state);
		thisRef = luaL_ref (state, LUA_REGISTRYINDEX);

		// Save the "this" table to index 0 of the "this" table
		HgLuaRestoreStack rs (vm);
		lua_rawgeti (state, LUA_REGISTRYINDEX, thisRef);
		lua_pushlightuserdata (state, (void *) this);
		lua_rawseti (state, -2, 0);
	}
}

//! Destructor
HgLuaScript::~HgLuaScript ()
{
	HgLuaRestoreStack rs (vm);

	if (vm.ok ())
	{
		lua_State *state = (lua_State*) vm;
		// Get the reference "this" table
		lua_rawgeti (state, LUA_REGISTRYINDEX, thisRef);

		// Clear index 0
		lua_pushnil (state);
		lua_rawseti (state, -2, 0);
	}
}

//! Compiles a buffer
bool HgLuaScript::compileBuffer (const char *pbBuffer, size_t szLen)
{
	if (pbBuffer != NULL)
		if (szLen != 0)
			if (vm.ok ())
			{
				lua_State *state = (lua_State*) vm;

				// Make sure we have the correct "this" table
				HgLuaThis luaThis (vm, thisRef);

				return vm.runBuffer (pbBuffer, szLen, NULL);
			}
	return false;
}

//! Compiles a file
bool HgLuaScript::compileFile (const char *strFilename)
{
	if (strFilename != NULL)
		if (vm.ok ())
		{
			// Make sure we have the correct "this" table
			HgLuaThis luaThisTable (vm, thisRef);

			return vm.runFile (strFilename);
		}
	return false;
}

//! Registers a method with Lua
int HgLuaScript::registerFunction (const char *funcName)
{
	if (funcName != NULL)
		if (vm.ok ())
		{
			int methodIdx = -1;

			HgLuaRestoreStack rs (vm);

			lua_State *state = (lua_State*) vm;

			methodIdx = ++numMethods;

			// Register a function with the lua script. Added it to the "this" table
			lua_rawgeti (state, LUA_REGISTRYINDEX, thisRef);

			// Push the function and parameters
			lua_pushstring (state, funcName);
			lua_pushnumber (state, (lua_Number) methodIdx);
			lua_pushcclosure (state, callback, 1);
			lua_settable (state, -3);

			return methodIdx;

		}

	return -1;
}

//! Selects a script function
bool HgLuaScript::selectScriptFunction (const char *funcName)
{
	if (funcName != NULL)
		if (vm.ok ())
		{
			lua_State *state = (lua_State*) vm;
			// Look up function name
			lua_rawgeti (state, LUA_REGISTRYINDEX, thisRef);
			lua_pushstring (state, funcName);
			lua_rawget (state, -2);
			lua_remove (state, -2);

			// Put the "this" table back
			lua_rawgeti (state, LUA_REGISTRYINDEX, thisRef);

			// Check that we have a valid function
			if (!lua_isfunction (state, -2))
			{
				lua_pop (state, 2);
				return false;
			}
			else
			{
				numArgs = 0;
				functionName = funcName;
			}
			return true;
		}
	return false;
}

//! Check if a function exists
bool HgLuaScript::scriptHasFunction (const char *scriptName)
{
	if (scriptName != NULL)
		if (vm.ok ())
		{
			HgLuaRestoreStack rs (vm);
			lua_State *state = (lua_State*) vm;
			lua_rawgeti (state, LUA_REGISTRYINDEX, thisRef);
			lua_pushstring (state, scriptName);
			lua_rawget (state, -2);
			lua_remove (state, -2);

			if (lua_isfunction (state, -1))
				return true;
		}
	return false;
}

//! Add an int arg
void HgLuaScript::addArg (int integer)
{
   if (vm.ok ())
   {
		lua_pushnumber ((lua_State*) vm, (lua_Number) integer);
		numArgs++;
   }
}

//! Add an float arg
void HgLuaScript::addArg (float floating)
{
	if (vm.ok ())
	{
		lua_pushnumber ((lua_State*) vm, (lua_Number) floating);
		numArgs++;
	}
}

//! Add a string arg
void HgLuaScript::addArg (char *string)
{
	if (vm.ok ())
	{
		lua_pushstring ((lua_State*) vm, string);
		numArgs++;
	}
}


//! Runs the script function
bool HgLuaScript::run (int numReturns)
{
	if (vm.ok ())
		if (vm.callFunction (numArgs + 1, numReturns) && numReturns > 0)
		{
			// Check for returns
			handleReturns (vm, functionName);
			lua_pop ((lua_State *) vm, numReturns);
			return true;
		}

	return false;
}

}

}
