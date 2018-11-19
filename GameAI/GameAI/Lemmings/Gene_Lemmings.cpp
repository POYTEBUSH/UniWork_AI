#include "Gene_Lemmings.h"

LemmingMoveChromosome::~LemmingMoveChromosome()
{
	LemmingMove* nextNode = mParentNode;

	while (nextNode) {
		LemmingMove* deletionNode = nextNode;
		nextNode->pNext;
		delete deletionNode;
	}
}

void LemmingMoveChromosome::AddMove(LemmingMove* move)
{
	mIndex++;
	move->Index = mIndex;
	move->pNext = mParentNode;
	move->StateSet = false;

	mParentNode = move;
	mCurrentNode = mParentNode;
}

LemmingMove * LemmingMoveChromosome::GetNextMove()
{
	if (mCurrentNode->pNext != NULL)
		mCurrentNode = mCurrentNode->pNext;
	else
		return nullptr;
	return mCurrentNode;
}
