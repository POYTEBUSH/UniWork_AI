//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _OBJECT_LEMMINGS_H
#define _OBJECT_LEMMINGS_H

#include "../Commons.h"
#include "Commons_Lemmings.h"
#include "Constants_Lemmings.h"
#include <vector>
#include <SDL.h>

using namespace::std;

class Texture2D;
class Character_Lemmings;

//------------------------------------------------------------------------

class ObjectManager_Lemmings
{
	//------------------------------------------------------------------------
public:
	void Init(SDL_Renderer* renderer);
	~ObjectManager_Lemmings();

	static ObjectManager_Lemmings* Instance();

	void Render();
	void Update(size_t deltaTime, SDL_Event e);

	void ExplosionAt(Vector2D position);


//------------------------------------------------------------------------
private:
	ObjectManager_Lemmings();
	static ObjectManager_Lemmings*		mInstance;

//------------------------------------------------------------------------
public:
	void RestartLevel();

	void SpawnALemming();
	void DetonateAllLemmings();

	void DecreaseSpawnDuration();
	void IncreaseSpawnDuration();
	int  GetSpawnDurationAsPercentage()											{ return (int)((mSpawnDuration / kMaxSpawnTime)*100.0f); }

	bool AllLemmingsHaveSpawned();
	int  GetNumberOfDeadLemmings();
	void AddADeadLemming()														{ mNumberOfDeadLemmings++; }

	Vector2D GetStartPosition()													{ return mEntrancePosition; }
	Vector2D GetEndPosition()													{ return mExitPosition; }

//------------------------------------------------------------------------
private:
	SDL_Renderer*				mRenderer;
	float						mSpawnDelay;
	float						mSpawnDuration;

	Texture2D*					mEntranceImage;
	Vector2D					mEntrancePosition;
	Texture2D*					mExitImage;
	Vector2D					mExitPosition;

//------------------------------------------------------------------------
public:
	Character_Lemmings*			mLemmings[kNumberOfLemmings];
	unsigned int				mCurrentNumberOfLemmings;
	unsigned int				mNumberOfDeadLemmings;
};

//------------------------------------------------------------------------

#endif //ObjectManager_Lemmings