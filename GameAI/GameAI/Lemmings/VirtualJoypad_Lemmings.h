//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _VIRTUALJOYPAD_LEMMINGS_H
#define _VIRTUALJOYPAD_LEMMINGS_H

#include <SDL.h>
#include "../Commons.h"

//--------------------------------------------------------------------------------------------------
class VirtualJoypad_Lemmings
{
	
//--------------------------------------------------------------------------------------------------
public:
	~VirtualJoypad_Lemmings();

	static VirtualJoypad_Lemmings* Instance();

	void SetInputState(SDL_Event e);
	void ClickedHandled()													{Clicked = false;}

//--------------------------------------------------------------------------------------------------
private:
	VirtualJoypad_Lemmings();

//--------------------------------------------------------------------------------------------------
private:
	static VirtualJoypad_Lemmings* mInstance;

public:
	Vector2D MousePosition;
	bool	 Clicked;
};

//--------------------------------------------------------------------------------------------------
#endif //_VIRTUALJOYPAD_LEMMINGS_H