/*****************************************************

	luaScript.h
		Lua script

*****************************************************/

#ifndef LUASCRIPT_H_INCLUDED
#define LUASCRIPT_H_INCLUDED

#include <lua/luainc.h>
#include "luavirtualmachine.h"

namespace hg {

namespace script {

class HgLuaScript {

public:

	/**
	* \brief Constructor
	*
	* \param vm Virtual machine
	*/
	HgLuaScript (HgLuaVirtualMachine& vm);

	/**
	* \brief Destructor
	*
	*/
	virtual ~HgLuaScript ();

	/**
	* \brief Execute a function in a LUA script
	*
	* \param funcName Name of function
	* \param ret Script return flag?
	* \param lnumArgs Number of paramaters
	* \param ... Paramater list
	*/
	void call (const char *funcName, int ret, int lnumArgs, ...);

	/**
	* \brief Compile script into the Virtual Machine
	*
	* \param filename Name of script
	* \return error condition
	*/
	bool compileFile (const char *filename);

	/**
	* \brief Compile script into the Virtual Machine
	*
	* \param pbBuffer Script buffer
	* \param szLen Length of buffer
	* \return error condition
	*/
	bool compileBuffer (const char *pbBuffer, size_t szLen);

	/**
	* \brief Register a function with Lua
	*
	* \param funcName Name of function to register
	* \return Function index, -1 if error
	*/
	int registerFunction (const char *funcName);

	/**
	* \brief Selects a Lua Script function to call
	*
	* \param filename Name of function
	* \return error condition (true or false)
	*/
	bool selectScriptFunction (const char *funcName);

	/**
	* \brief Add an argument to the Lua stack
	*
	* \param integer Integer argument
	*/
	void addArg (int integer);

	/**
	* \brief Add an argument to the Lua stack
	*
	* \param floating Floating point argument
	*/
	void addArg (float floating);

	/**
	* \brief Add an argument to the Lua stack
	*
	* \param string String argument
	*/
	void addArg (char *string);

	/**
	* \brief Executes loaded script
	*
	* \param Number of returns, 0 is default
	* \return true on success, false on error
	*/
	bool run (int Returns = 0);

	/**
	* \brief Checks if script has function
	*
	* \param Script name
	* \return true if script has a function, false if not
	*/
	bool scriptHasFunction (const char *scriptName);

	/**
	* \brief Method indexing check
	*
	* \return Number of methods
	*/
	int methods (void) {
		return numMethods;
	}

	/**
	* \brief When the script calls a class method, this is called
	*
	* \param vm Lua virtual machine
	* \param functionNumber Function number
	* \return ?
	*/
	virtual int scriptCalling (HgLuaVirtualMachine& vm, int functionNumber) = 0;

	/**
	* \brief When the script function has returns
	*
	* \param vm Lua virtual machine
	* \param func Function name?
	*/
	virtual void handleReturns (HgLuaVirtualMachine& vm, const char *func) = 0;

	/**
	* \brief Returns Lua virtual machine
	*
	* \return virtual machine
	*/
	inline HgLuaVirtualMachine& getvm () const {
		return vm;
	}

protected:

	//! Number of methods
	int numMethods;

	//! Virtual machine
	HgLuaVirtualMachine& vm;

	//! This reference
	int thisRef;

	//! Number of arguments
	int numArgs;

	//! Function name
	const char *functionName;
};

}

}

#endif
