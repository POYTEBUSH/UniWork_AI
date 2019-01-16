#pragma once
#include <vector>
#include "Gene_Lemmings.h"
#include <string>
#include "../TinyXML/tinystr.h"
#include "../TinyXML/tinyxml.h"

class GeneFileManager
{
public:
	GeneFileManager(int Level);
	
	///<summary>Adds item to file and then saves</summary>
	void AddMove(LemmingMoveChromosome* move);
	void SaveFile();

	LemmingMoveChromosome* GetHighestChromosome() { 
		if(mAllMoves.size() > 0 && mAllMoves[mHighestIndex])
			return mAllMoves[mHighestIndex]; 
	};
	LemmingMoveChromosome* GetLatestChromosome() { 
		if (mAllMoves.size() > 0 && mAllMoves[mAllMoves.size() - 1])
			return mAllMoves[mAllMoves.size() - 1]; 
	};

	void CheckHighscore();

	~GeneFileManager() {};

private:

	int mLevel;
	int mHighestIndex;
	std::vector<LemmingMoveChromosome*> mAllMoves;
};

