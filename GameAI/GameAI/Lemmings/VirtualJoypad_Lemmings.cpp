//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "VirtualJoypad_Lemmings.h"

//Initialise the instance to null.
VirtualJoypad_Lemmings* VirtualJoypad_Lemmings::mInstance = NULL;

//--------------------------------------------------------------------------------------------------

VirtualJoypad_Lemmings::VirtualJoypad_Lemmings()
{
	MousePosition.Zero();
	Clicked = false;
}

//--------------------------------------------------------------------------------------------------

VirtualJoypad_Lemmings::~VirtualJoypad_Lemmings()
{
	mInstance = NULL;
}

//--------------------------------------------------------------------------------------------------

VirtualJoypad_Lemmings* VirtualJoypad_Lemmings::Instance()
{
	if(!mInstance)
	{
		mInstance = new VirtualJoypad_Lemmings;
	}

	return mInstance;
}

//--------------------------------------------------------------------------------------------------

void VirtualJoypad_Lemmings::SetInputState(SDL_Event e)
{
	switch (e.type)
	{
		case SDL_MOUSEMOTION:
			MousePosition.x = e.button.x;
			MousePosition.y = e.button.y;
		break;

		case SDL_MOUSEBUTTONDOWN:
			switch (e.button.clicks)
			{
				case 1:
					Clicked = true;
				break;

				default:
				break;
			}
		break;

		case SDL_MOUSEBUTTONUP:
			Clicked = false;
		break;

		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------