//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _GAMESCREEN_LEMMINGS_H
#define _GAMESCREEN_LEMMINGS_H

#include "../GameScreen.h"
#include "../Commons.h"
#include "Commons_Lemmings.h"
#include "Constants_Lemmings.h"
#include "Gene_Lemmings.h"
#include <SDL.h>

using namespace::std;

//--------------------------------------------------------------------------------------------------

class GameScreen_Lemmings : GameScreen
{
//--------------------------------------------------------------------------------------------------
public:
	GameScreen_Lemmings(SDL_Renderer* renderer);
	~GameScreen_Lemmings();

	void Render();
	void Update(size_t deltaTime, SDL_Event e);

//--------------------------------------------------------------------------------------------------
private:
	void HandlePlayerInput();
	void RestartLevel(int level);

//--------------------------------------------------------------------------------------------------
private:
	bool mPause;
	bool mAIOn;
};

//--------------------------------------------------------------------------------------------------



#endif //_GAMESCREEN_LEMMINGS_H