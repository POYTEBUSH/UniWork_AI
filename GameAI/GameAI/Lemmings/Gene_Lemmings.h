#pragma once
#include "Commons_Lemmings.h" 
#include "Character_Lemmings.h"
#include <string>
#include <sstream>

enum LemmingMoveType {
	ScreenPos,
	LemmingPos,
};

struct LemmingMove {

	int MoveValue;
	void* ClickTarget;
	LemmingMoveType MoveType;
	float TimeDelay;
	LEMMING_TYPE LemmingType;

	bool StateSet;
	int Index;
	LemmingMove* pNext;
};


class LemmingMoveChromosome
{
public:

	LemmingMoveChromosome() { mIndex = 0; mCurrentNode = nullptr; mParentNode = nullptr; }
	~LemmingMoveChromosome();

	float GetScore() const { return mScore; };
	void SetScore(float i) { mScore = i; };

	int GetItteration() const { return mItteration; };
	void SetItteration(int i) { mItteration = i; };

	int GetTestItteration() const { return mGeneTestNumber; };
	void SetTestItteration(int i) { mGeneTestNumber = i; };

	int GetCompletedLemmmingCount() const { return mNumberOfCompletedLemmings; };
	void SetCompletedLemmmingCount(int i) { mNumberOfCompletedLemmings = i; };

	void SetRoutineInt(std::vector<int> routine) { mRoutineItemClicks = routine; };
	std::vector<int> GetRoutineInt() const { return mRoutineItemClicks; };

	void AddMove(LemmingMove* move);
	LemmingMove* GetNextMove();

private:
	LemmingMove* mParentNode;
	LemmingMove* mCurrentNode;
	int mGeneTestNumber;
	int mIndex;
	int mItteration;
	float mScore;
	int mNumberOfCompletedLemmings;
	std::vector<int> mRoutineItemClicks;
};