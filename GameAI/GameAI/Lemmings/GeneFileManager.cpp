#include "GeneFileManager.h"

GeneFileManager::GeneFileManager(int Level)
{
	mLevel = Level;
	mHighestIndex = 0;
	mAllMoves.clear();

	TiXmlDocument doc("GeneRoutines_" + to_string(mLevel) + ".xml");
	if (doc.LoadFile())
	{
		TiXmlHandle hDoc(&doc);
		TiXmlElement *pRoot, *pParm;
		pRoot = doc.FirstChildElement("GeneLevel");

		int maxX = 0, maxY = 0;
		if (pRoot)
		{
			for (TiXmlElement* e = pRoot->FirstChildElement("GeneData"); e != NULL; e = e->NextSiblingElement("GeneData"))
			{	
				LemmingMoveChromosome* move = new LemmingMoveChromosome;

				int itteration = 0, completed = 0, testitteration;
				float genescore = 0.f;
				vector<int> GeneRoutine;
				string geneRoutineString;

				e->QueryIntAttribute("TestNum", &testitteration);
				e->QueryIntAttribute("Itteration", &itteration);
				e->QueryIntAttribute("Completed", &completed);
				e->QueryFloatAttribute("GeneScore", &genescore);
				e->QueryStringAttribute("GeneRoutine", &geneRoutineString);

				//Convert the XML Gene routine to vector of ints
				for (size_t i = 0; i < geneRoutineString.size(); i++)
				{					 
					GeneRoutine.push_back((int)geneRoutineString[i] - '0');
				}

				move->SetTestItteration(testitteration);
				move->SetItteration(itteration);
				move->SetRoutineInt(GeneRoutine);
				move->SetScore(genescore);
				move->SetCompletedLemmmingCount(completed);

				mAllMoves.push_back(move);
			}
			CheckHighscore();
		}
	}
	else {
		std::cout << "Doc failed to read";
	}
}

void GeneFileManager::AddMove(LemmingMoveChromosome * move)
{
	move->SetItteration(mAllMoves.size());
	mAllMoves.push_back(move);
	CheckHighscore();
}

void GeneFileManager::SaveFile()
{
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc.LinkEndChild(decl);
	TiXmlElement * mainRoot = new TiXmlElement("GeneLevel");
	mainRoot->SetAttribute("Level", mLevel);

	for (size_t i = 0; i < mAllMoves.size(); i++)
	{
		auto move = mAllMoves[i];
		TiXmlElement * root = new TiXmlElement("GeneData");
		root->SetAttribute("Itteration", move->GetItteration());
		root->SetAttribute("TestNum", move->GetTestItteration());
		root->SetAttribute("Completed", move->GetCompletedLemmmingCount());
		root->SetAttribute("GeneScore", move->GetScore());
		auto routine = move->GetRoutineInt();
		stringstream ss;
		for (size_t i = 0; i < routine.size(); i++)
		{
			ss << routine[i];
		}
		root->SetAttribute("GeneRoutine", ss.str());
		mainRoot->LinkEndChild(root);
	}

	doc.LinkEndChild(mainRoot);
	doc.SaveFile("GeneRoutines_" + to_string(mLevel) + ".xml");
}

void GeneFileManager::CheckHighscore()
{
	//Best routine each itteration is the one with the highest completion otherwise the highest score
	auto highscore = 0; auto highscoreIndex = 0; auto highestCompletion = 0;
	for (size_t i = 0; i < mAllMoves.size(); i++)
	{
		if (mAllMoves[i]->GetScore() > highscore || mAllMoves[i]->GetCompletedLemmmingCount() > highestCompletion) {
			highscore = mAllMoves[i]->GetScore();
			highestCompletion = mAllMoves[i]->GetCompletedLemmmingCount();
			highscoreIndex = i;
		}
	}
	mHighestIndex = highscoreIndex;
}
