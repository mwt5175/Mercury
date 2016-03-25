/*****************************************************

	state.h
		State management

*****************************************************/

#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <core/timer.h>

namespace mercury {

class CApplication;

/**
*	Base application state class.
*/
class CState {
	friend class CStateMachine;
protected:
	/**
	*	local time keeps track of how long state has been running
	*/
	timer localTime;
	/**
	*	state id
	*/
	unsigned int id;

public:

	CState ();
	virtual ~CState ();
	/**
	* Initialize state
	* \param Application pointer
	*/
	virtual void Initialize (CApplication*) = 0;
	/**
	* Shutdown state
	* \param Application pointer
	*/
	virtual void Shutdown   (CApplication*) = 0;
	/**
	* Update for next frame
	* \param Application pointer
	*/
	virtual bool Update     (CApplication*) = 0;
	/**
	* Render state
	* \param Application pointer
	*/
	virtual bool Render     (CApplication*) = 0;
};

/**
* Base state factory class. The application must inherit for
* creating different run time states
*/
class CStateFactory {
public:
	virtual CState* Get (unsigned int stateId) const {
		HG_UNREFERENCED (stateId);
		return 0;
	}
};

class CStateMachine {
	CState* currentState;

public:

	CStateMachine () : currentState(0) {}

	bool SetState (CApplication* app, CState* state, unsigned int id) {
		currentState = state;
		if (!state)
			return true; // null states
		state->id = id;
		state->Initialize (app);
		return true;
	}

	bool FreeCurrentState (CApplication* app) {
		if (!currentState)
			return true; // already freed
		currentState->Shutdown (app);
		delete currentState;
		currentState = 0;
		return true;
	}
};

}

#endif
