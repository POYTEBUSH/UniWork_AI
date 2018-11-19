//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _UIMANAGER_LEMMINGS_H
#define _UIMANAGER_LEMMINGS_H

#include "../Commons.h"
#include "Commons_Lemmings.h"
#include <vector>
#include <SDL.h>

using namespace::std;

class Texture2D;

//------------------------------------------------------------------------

class UIManager_Lemmings
{
//------------------------------------------------------------------------
public:
	void Init(SDL_Renderer* renderer);
	~UIManager_Lemmings();
	
	static UIManager_Lemmings* Instance();

	void Render();
	void Update(size_t deltaTime, SDL_Event e);

	void RefreshHighlight();

//------------------------------------------------------------------------
private:
	UIManager_Lemmings();

	void DrawDebugCircle(Vector2D centrePoint, double radius, int red, int green, int blue);

//------------------------------------------------------------------------
private:
	static UIManager_Lemmings*		mInstance;

	SDL_Renderer*					mRenderer;

	Texture2D*						mIcons;
	Texture2D*						mIconHighlight;
	Texture2D*						mIconNumbers;

	Texture2D*						mScorePanel;
	Texture2D*						mGreenNumbers;
};

//------------------------------------------------------------------------

#endif //_UIMANAGER_LEMMINGS_H