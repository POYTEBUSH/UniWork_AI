#include "StateManager_Lemmings.h"

#pragma region Static References
StateManager_Lemmings* StateManager_Lemmings::mInstance = NULL;
vector<int> StateManager_Lemmings::mLemmingsOptionsAvailable;
int StateManager_Lemmings::mAllocationNumber;
int StateManager_Lemmings::mInterations = 0;
int StateManager_Lemmings::mNumOfGenePoolsTested = 0;

b013432fHelper* StateManager_Lemmings::Helpers;
vector<LemmingMoveChromosome*> StateManager_Lemmings::mGenePool;
LemmingMoveChromosome* StateManager_Lemmings::mCurrentRoutine;
LemmingMoveChromosome* StateManager_Lemmings::mBestRoutine;

Character_Lemmings* StateManager_Lemmings::mCurrentUsedLemming;
int StateManager_Lemmings::mCurrentUsedIndex = 0;

LemmingMove* StateManager_Lemmings::mCurrentMove;
double StateManager_Lemmings::mChromosomeElapsed;

StateManager_Lemmings::StateManager_Lemmings() { }
StateManager_Lemmings::~StateManager_Lemmings() { }

LemmingMoveChromosome * StateManager_Lemmings::GetNextGeneFromPool()
{
	mChromosomeElapsed = 0;
	mCurrentUsedLemming = nullptr;
	auto returnVal = mGenePool[mInterations];
	mInterations++;
	return returnVal;
}

GeneFileManager* StateManager_Lemmings::mFileManager;
#pragma endregion

void StateManager_Lemmings::Init(int level)
{
	mFileManager = new GeneFileManager(level);
	mBestRoutine = mFileManager->GetHighestChromosome();

	if (mBestRoutine != nullptr) {
		cout << "Continuing with previous best routine " << mBestRoutine->GetItteration() << " -> " << mBestRoutine->GetScore() << "(" << mBestRoutine->GetCompletedLemmmingCount() << ")" << endl;
	}

	mAllocationNumber = 0;
	for (size_t i = 0; i < (LEMMINGTYPE_MAX - 1); i++)
	{
		auto numOfType = GameManager_Lemmings::Instance()->mIconQuantities[i];
		if (numOfType > 0) {
			for (size_t j = 0; j < numOfType; j++)
			{
				mLemmingsOptionsAvailable.push_back(i);
			}
		}
	}
	GenerateGenePool();

	mCurrentRoutine = GetNextGeneFromPool();
	mCurrentMove = mCurrentRoutine->GetNextMove();
}

//Gets the next lemming that is available for a type to be set, If none are different then the same lemming is used.
void StateManager_Lemmings::GetNextAvailableLemming()
{
	for (size_t i = 0; i < kNumberOfLemmings; i++)
	{
		auto lemming = ObjectManager_Lemmings::Instance()->mLemmings[i];
		if (lemming->HasSpawned() && lemming->IsAlive() && lemming != mCurrentUsedLemming) {
			mCurrentUsedLemming = lemming;
			continue;
		}
	}
	if (mCurrentUsedLemming == NULL)
		mCurrentUsedLemming = ObjectManager_Lemmings::Instance()->mLemmings[0];
}

void StateManager_Lemmings::Update(size_t DeltaTime)
{
	AddToGenePoolScore((float)(ObjectManager_Lemmings::Instance()->mCurrentNumberOfLemmings - ObjectManager_Lemmings::Instance()->mNumberOfDeadLemmings) / (float)DeltaTime);

	if (mCurrentMove != NULL) {
		if (mChromosomeElapsed >= mCurrentMove->TimeDelay) {
			mChromosomeElapsed = 0;
			mCurrentMove = mCurrentRoutine->GetNextMove();
		}
		else {
			Vector2D pos;
			if (mCurrentMove->MoveType == ScreenPos) {
				pos = *(Vector2D*)mCurrentMove->ClickTarget;
			}
			else if(mCurrentMove->MoveType == LemmingPos) {
				GetNextAvailableLemming();
				//If Lemming has spawned then do action, else skip
				if (mCurrentUsedLemming->HasSpawned()) {

					auto lemming = mCurrentUsedLemming;
					if (lemming->IsAlive())
						pos = lemming->GetCentralPosition();
				}
			}

			VirtualJoypad_Lemmings::Instance()->MousePosition.y = pos.y;
			VirtualJoypad_Lemmings::Instance()->MousePosition.x = pos.x;
			if (!mCurrentMove->StateSet) {
				VirtualJoypad_Lemmings::Instance()->Clicked = true;
				mCurrentMove->StateSet = true;
			}
		}
		mChromosomeElapsed++;
	}
}

void StateManager_Lemmings::RestartLevel()
{
	int score = 0; int completed = 0;
	for (size_t i = 0; i < kNumberOfLemmings; i++)
	{
		if (ObjectManager_Lemmings::Instance()->mLemmings[i]->IsInState(LEMMINGSTATE_EXIT)) {
			completed++;
			score += 2500;
		}
	}
	mCurrentRoutine->SetCompletedLemmmingCount(completed);
	AddToGenePoolScore(score);
	mFileManager->AddMove(mCurrentRoutine);
	mFileManager->SaveFile();

	Helpers->PrintTitle("Gene Testing " + to_string(mNumOfGenePoolsTested));

	stringstream ss;
	ss << "Gene Finished -> Score: " << mCurrentRoutine->GetScore();
	Helpers->PrintContentText(ss.str());

	if (StateManager_Lemmings::GetGenePoolInteration() < StateManager_Lemmings::GetGenePoolSize()) {
		mCurrentRoutine = StateManager_Lemmings::GetNextGeneFromPool();
	}
	else {
		Helpers->PrintContentText("Gene pool processed");

		//Best routine each itteration is the one with the highest completion otherwise the highest score
		auto highscore = 0; auto highscoreIndex = 0; auto highestCompletion = 0;
		for (size_t i = 0; i < mGenePool.size(); i++)
		{
			if (mGenePool[i]->GetScore() > highscore || mGenePool[i]->GetCompletedLemmmingCount() > highestCompletion) {
				highscore = mGenePool[i]->GetScore();
				highestCompletion = mGenePool[i]->GetCompletedLemmmingCount();
				highscoreIndex = i;
			}
		}
		mBestRoutine = mGenePool[highscoreIndex];

		stringstream ss;
		ss << "Highscore: " << highscore;
		Helpers->PrintContentText(ss.str());

		GenerateGenePool();
		mCurrentRoutine = GetNextGeneFromPool();		
	}
}

void StateManager_Lemmings::AddToGenePoolScore(float i)
{
	if (mCurrentRoutine != nullptr) {
		float currentVal = mCurrentRoutine->GetScore();
		auto newScore = currentVal += i;
		mCurrentRoutine->SetScore(newScore);
	}
}

void StateManager_Lemmings::CalculateDistanceScore()
{
	int totalDistance = 0;
	auto endPos = Landscape_Lemmings::Instance()->mEndPosition;
	auto startPos = Landscape_Lemmings::Instance()->mStartPosition;

	for (size_t i = 0; i < kNumberOfLemmings; i++)
	{
		auto lemming = ObjectManager_Lemmings::Instance()->mLemmings[i];
		auto centreLemmming = lemming->GetCentralPosition();

		auto distFromEnd = Helpers->VectorDistance(centreLemmming, endPos);

		auto distScore = abs(distFromEnd);

		totalDistance -= distScore;
	}
	totalDistance /= kNumberOfLemmings;

	cout << to_string(totalDistance) << endl;
	AddToGenePoolScore(totalDistance);
}

void StateManager_Lemmings::GenerateGenePool()
{
	Helpers->PrintContentText("");
	Helpers->PrintTitle("Gene Generations " + to_string(mNumOfGenePoolsTested));

	mGenePool.clear();

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	// randomly shuffle the order of the LemmingsOptions pool
	std::shuffle(mLemmingsOptionsAvailable.begin(), mLemmingsOptionsAvailable.end(), std::default_random_engine(seed));

	//First pool takes in random values
	vector<int> options = mLemmingsOptionsAvailable;
	auto gene1 = GenerateGene(0, options);

	//Second takes in first's options but reversed
	std::shuffle(mLemmingsOptionsAvailable.begin(), mLemmingsOptionsAvailable.end(), std::default_random_engine(seed));
	auto gene2 = GenerateGene(1, mLemmingsOptionsAvailable);

	//For the last two genes take alternating values

	vector<int> subOption = Helpers->GetAlternatingValues(mLemmingsOptionsAvailable, options);
	auto gene3 = GenerateGene(2, subOption);
	subOption = Helpers->GetAlternatingValues(options, mLemmingsOptionsAvailable);
	auto gene4 = GenerateGene(3, subOption);

	//Make sure scores all start at 0
	gene1->SetScore(0);
	gene2->SetScore(0);
	gene3->SetScore(0);
	gene4->SetScore(0);

	mNumOfGenePoolsTested++;

	gene1->SetTestItteration(mNumOfGenePoolsTested);
	gene2->SetTestItteration(mNumOfGenePoolsTested);
	gene3->SetTestItteration(mNumOfGenePoolsTested);
	gene4->SetTestItteration(mNumOfGenePoolsTested);

	mGenePool.push_back(gene1);
	mGenePool.push_back(gene2);
	mGenePool.push_back(gene3);
	mGenePool.push_back(gene4);

	mInterations = 0;
	Helpers->PrintFooter();
}

LemmingMoveChromosome* StateManager_Lemmings::GenerateGene(int index, vector<int> options)
{
	LemmingMoveChromosome* chromosome = new LemmingMoveChromosome();

	int count = 0, lemmingIndex = 0;

	//First time run the best Routine from previous will not exist so randomly generate two parents	
	if (mBestRoutine != NULL && mBestRoutine->GetScore() > 0 && index == 0)
	{
		chromosome->SetRoutineInt(mBestRoutine->GetRoutineInt());
		CreateGeneFromRoutineInt(chromosome, mBestRoutine->GetRoutineInt());
	}
	else {
		chromosome->SetRoutineInt(options);
		CreateGeneFromRoutineInt(chromosome, options);
	}

	mAllocationNumber++;
	chromosome->SetItteration(mAllocationNumber);
	return chromosome;
}

void StateManager_Lemmings::CreateGeneFromRoutineInt(LemmingMoveChromosome* chromosome, vector<int> routine)
{
	int iconTypeOffset = IconPanelIndividualWidth * 2; //This caters for the + and - icons, which arent selections for lemming type.
	for (size_t i = 0; i < routine.size(); i++)
	{
		auto actionPos = new Vector2D;
		actionPos->x = iconTypeOffset + IconPanelIndividualWidth * routine[i] + IconPanelIndividualWidth / 2;
		actionPos->y = 500;

		//No delay set for when setting the type
		auto move1 = new LemmingMove;
		move1->ClickTarget = actionPos;
		move1->MoveType = ScreenPos;
		move1->MoveValue = i;
		move1->TimeDelay = 500/16;
		chromosome->AddMove(move1);

		auto move2 = new LemmingMove;
		move2->MoveType = LemmingPos;
		move2->TimeDelay = 2500/16;
		chromosome->AddMove(move2);
	}
}
