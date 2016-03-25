/*****************************************************

	luaVirtualMachine.h
		Lua Virtual Machine

*****************************************************/

#ifndef LUAVIRTUALMACHINE_H_INCLUDED
#define LUAVIRTUALMACHINE_H_INCLUDED

#include <lua/luainc.h>
#include <string>

namespace hg {

namespace script {

/**
* \brief LUA Virtual Machine
*
*/
class HgLuaVirtualMachine
{
public:

	/**
	* \brief Constructor
	*
	*/
	HgLuaVirtualMachine ();

	/**
	* \brief Destructor
	*
	*/
	~HgLuaVirtualMachine ();

	/**
	* \brief Load and run script elements
	*
	* \param strFilename Script file
	* \return true on success, false on error
	*/
	bool runFile (std::string strFilename);

	/**
	* \brief Load and run script elements
	*
	* \param pbBuffer Script buffer
	* \param szLen Length of buffer
	* \param strName Script name
	* \return true on success, false on error
	*/
	bool runBuffer (const char *pbBuffer, size_t szLen, std::string strName);

	/**
	* \brief Displays Lua error code
	*
	*/
	void luaError (int iErrorCode);

	/**
	* \brief C-Api into script
	*
	* \param nArgs Argument count?
	* \param nReturns Number of results?
	* \returns true on success, false on error
	*/
	bool callFunction (int nArgs, int nReturns = 0);

	/**
	* \brief Returns Lua state
	*
	* \returns current lua_State
	*/
	operator lua_State *(void) {
		return vmState;
	}

	/**
	* \brief Lua panic
	*
	* \param lua Lua state
	*/
	static void panic (lua_State *lua);

	/**
	* \brief Check if the VM is OK (opened correctly)
	*
	* \returns true on success, false on error
	*/
	bool ok () {
		return vmIsOk;
	}

protected:

	//! Virtual machine state
	lua_State *vmState;

	//! Virtual Machine Ok flag
	bool vmIsOk;
};

}

}

#endif
