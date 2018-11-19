#pragma once
#include "Character_Lemmings.h"
#include "Constants_Lemmings.h"
#include "ObjectManager_Lemmings.h"
#include "GameManager_Lemmings.h"
#include "Commons_Lemmings.h"
#include "Gene_Lemmings.h"
#include "../b013432fHelper.h"
#include <sstream>
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include <stdlib.h>     //for using the function sleep
#include "VirtualJoypad_Lemmings.h"
#include <vector>
#include "Landscape_Lemmings.h"

#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "GeneFileManager.h"

#define GENEPOOL_SIZE 4

class StateManager_Lemmings
{
public:

	void Init(int lemming);

	~StateManager_Lemmings();

	static StateManager_Lemmings* Instance() { return mInstance; }

	LemmingMoveChromosome* GetNextGeneFromPool();

	static int GetGenePoolInteration() { return mInterations; }
	static int GetGenePoolSize() { return mGenePool.size(); }

	void GetNextAvailableLemming();
	void Update(size_t DeltaTime);
	void RestartLevel();

	void AddToGenePoolScore(float i);
	void CreateGeneFromRoutineInt(LemmingMoveChromosome* chromosome, vector<int> routine);

private:

	static int mInterations;

	void CalculateDistanceScore();
	void GenerateGenePool(); //Used to generate random parents at the start
	LemmingMoveChromosome* GenerateGene(int index, vector<int> options);

	int mLemmingsLevel;

	StateManager_Lemmings();
	static StateManager_Lemmings* mInstance;

	static Character_Lemmings* mCurrentUsedLemming;
	static int mCurrentUsedIndex;

	/// <summary> Available options to choose from (number of each available type) </summary>
	static vector<int> mLemmingsOptionsAvailable;
	/// <summary> Gene pool that will be tested this test iteration </summary>
	static vector<LemmingMoveChromosome*> mGenePool;
	/// <summary> Current routine that is being tested </summary>
	static LemmingMoveChromosome* mCurrentRoutine;
	/// <summary> Best routine so far, calculated at the end of each run </summary>
	static LemmingMoveChromosome* mBestRoutine;
	/// <summary> Number of gene pools in total that have been tested </summary>
	static int mNumOfGenePoolsTested;

	static int mAllocationNumber;

	static double mChromosomeElapsed;

	static LemmingMove* mCurrentMove;

	static b013432fHelper* Helpers;

	static GeneFileManager* mFileManager;

};

