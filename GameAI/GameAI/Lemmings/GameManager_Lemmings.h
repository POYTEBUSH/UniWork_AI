//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _GAMEMANAGER_LEMMINGS_H
#define _GAMEMANAGER_LEMMINGS_H

#include "../Commons.h"
#include "Commons_Lemmings.h"
#include <vector>
#include <SDL.h>

using namespace::std;

//------------------------------------------------------------------------

class GameManager_Lemmings
{
	//------------------------------------------------------------------------
public:
	void Init(int level);
	~GameManager_Lemmings();

	static GameManager_Lemmings* Instance();

	void		 RestartLevel();

	void		 Update(size_t deltaTime, SDL_Event e);

	void		 SetIconSelected(LEMMING_TYPE type)								{ mSelectedIcon = type; }
	LEMMING_TYPE GetSelectedIcon()												{ return mSelectedIcon; }
	void		 UseAnIcon()													{ mIconQuantities[mSelectedIcon]--; }

	void		 AddEscapedLemming()											{ mEscapedLemmings++; }
	unsigned int GetNumberOfEscapedLemmings();

//------------------------------------------------------------------------
private:
	GameManager_Lemmings();
	static GameManager_Lemmings*		mInstance;

	LEMMING_TYPE						mSelectedIcon;

	unsigned int						mEscapedLemmings;
//------------------------------------------------------------------------
public:
	unsigned int mIconQuantities[LEMMINGTYPE_MAX-1];
	unsigned int mLevelID;
	int			 mLevelTime;
};

//------------------------------------------------------------------------

#endif //_GAMEMANAGER_LEMMINGS_H