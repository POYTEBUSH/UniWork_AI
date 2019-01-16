//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _LANDSCAPE_LEMMINGS_H
#define _LANDSCAPE_LEMMINGS_H

#include "../Commons.h"
#include "Commons_Lemmings.h"
#include <SDL.h>

using namespace::std;

class Texture2D;

//------------------------------------------------------------------------

class Landscape_Lemmings
{
//------------------------------------------------------------------------
public:
	void Init(SDL_Renderer* renderer);
	~Landscape_Lemmings();
	void RestartLevel();

	static Landscape_Lemmings* Instance();

	void RenderDestructibleLand();
	void RenderSea();

	void Update(size_t deltaTime, SDL_Event e);
	
	void DigLand(int xCentrePos, int yCentrePos);
	void PunchLand(int xCentrePos, int yCentrePos);
	void PickAxeLand(int xCentrePos, int yCentrePos);
	void RemoveLandForExplosion(int xCentrePos, int yCentrePos);
	void AddLandForPlatform(int xPos, int yPos);

	LandscapeType_Lemmings GetGroundTypeAt(Vector2D position);
	LandscapeType_Lemmings GetGroundTypeAt(int x, int y);

//------------------------------------------------------------------------
private:
	Landscape_Lemmings();

	void CreateGroundImage();
	void ModifyLand(int xCentrePos, int yCentrePos, Texture2D & explosionArea);
	void RefreshLand();

	void SetupCollisionMap();
	void UpdateCollisionMap();

	Uint32 GetPixel(SDL_Surface *surface, int x, int y);
	void SetPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);

//------------------------------------------------------------------------
private:
	static Landscape_Lemmings*		mInstance;

	SDL_Renderer*					mRenderer;

	Texture2D*						mGroundTexture;
	Texture2D*						mReserveGroundTexture;
	Texture2D*						mWaveImage;
	bool							mLandModified;

	LandscapeType_Lemmings**		mCollisionMap;

	Texture2D*						mDigTexture;
	Texture2D*						mPunchTexture;
	Texture2D*						mPickAxeTexture;
	Texture2D*						mExplosionTexture;
	Texture2D*						mBuildPlatformTexture;

	Uint8*	r;
	Uint8*	g;
	Uint8*	b;

public:
	Vector2D						mStartPosition;
	Vector2D						mEndPosition;

};

//------------------------------------------------------------------------

#endif //_LANDSCAPE_LEMMINGS_H