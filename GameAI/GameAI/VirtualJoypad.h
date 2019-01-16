//------------------------------------------------------------------------
//  Author: Paul Roberts 2017
//------------------------------------------------------------------------

#ifndef _VIRTUALJOYPAD_H
#define _VIRTUALJOYPAD_H

#include <SDL.h>

//--------------------------------------------------------------------------------------------------
class VirtualJoypad
{
	
//--------------------------------------------------------------------------------------------------
public:
	~VirtualJoypad();

	static VirtualJoypad* Instance();

	void SetJoypadState(SDL_Event e);

	void ResetJoypadState();

//--------------------------------------------------------------------------------------------------
private:
	VirtualJoypad();

//--------------------------------------------------------------------------------------------------
private:
	static VirtualJoypad* mInstance;

public:
	bool LeftArrow;
	bool RightArrow;
	bool UpArrow;
	bool DownArrow;
	bool Fire;
};

//--------------------------------------------------------------------------------------------------
#endif //_VIRTUALJOYPAD_H