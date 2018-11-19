//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "UIManager_Lemmings.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "Constants_Lemmings.h"
#include "GameManager_Lemmings.h"
#include "ObjectManager_Lemmings.h"
#include "VirtualJoypad_Lemmings.h"
#include <assert.h>

//Initialise the instance to null.
UIManager_Lemmings* UIManager_Lemmings::mInstance = NULL;

//--------------------------------------------------------------------------------------------------

UIManager_Lemmings::UIManager_Lemmings()
{
	
}

//--------------------------------------------------------------------------------------------------

void UIManager_Lemmings::Init(SDL_Renderer* renderer)
{
	mRenderer = renderer;
	
	//Add the icons.
	mIcons = new Texture2D(mRenderer);
	mIcons->LoadFromFile("Images/Lemmings/UI/Icons.bmp");

	//And the highlight
	mIconHighlight = new Texture2D(mRenderer);
	mIconHighlight->LoadFromFile("Images/Lemmings/UI/IconHighlight.bmp");

	//And the numbers.
	mIconNumbers = new Texture2D(mRenderer);
	mIconNumbers->LoadFromFile("Images/Lemmings/UI/IconNumbers.bmp");

	//The Green score text.
	mScorePanel = new Texture2D(mRenderer);
	mScorePanel->LoadFromFile("Images/Lemmings/UI/ScorePanel.bmp");

	//And the numbers.
	mGreenNumbers = new Texture2D(mRenderer);
	mGreenNumbers->LoadFromFile("Images/Lemmings/UI/GreenNumbers.bmp");

}

//--------------------------------------------------------------------------------------------------

UIManager_Lemmings::~UIManager_Lemmings()
{
	delete mIcons;
	mIcons = NULL;

	delete mIconHighlight;
	mIconHighlight = NULL;

	delete mIconNumbers;
	mIconNumbers = NULL;
	
	delete mScorePanel;
	mScorePanel = NULL;

	mInstance = NULL;
}

//--------------------------------------------------------------------------------------------------

UIManager_Lemmings* UIManager_Lemmings::Instance()
{
	if (!mInstance)
	{
		mInstance = new UIManager_Lemmings;
	}

	return mInstance;
}

//--------------------------------------------------------------------------------------------------

void UIManager_Lemmings::RefreshHighlight()
{

}

//--------------------------------------------------------------------------------------------------

void UIManager_Lemmings::Render()
{
	//------------------------------------------------------------------------
	//Selectable Icons.
	//------------------------------------------------------------------------
	mIcons->Render(Vector2D(0.0f, kLemmingsPlayAreaHeight+kScoreHeight));
	int yIconOffset = kLemmingsPlayAreaHeight + kScoreHeight + IconYOffset+1;
	int xIconOffset = IconXOffset + (IconPanelIndividualWidth * 2);
	SDL_Rect portionOfSpritesheet;
	SDL_Rect destRect;

	//Draw speed of spawn - In values from 0 - 99.
	for (int i = 0; i < 2; i++)
	{
		int percentage = 100-min(ObjectManager_Lemmings::Instance()->GetSpawnDurationAsPercentage(), 99);

		//First digit.
		int w = percentage / 10;
		portionOfSpritesheet = { w*(int)(mIconNumbers->GetWidth() / 10), 0, (int)mIconNumbers->GetWidth() / 10, (int)mIconNumbers->GetHeight() };
		destRect = { IconXOffset+(IconPanelIndividualWidth*i), yIconOffset, (int)mIconNumbers->GetWidth() / 10, 30 };
		mIconNumbers->Render(portionOfSpritesheet, destRect);

		//Second digit.
		w = percentage % 10;
		portionOfSpritesheet = { w*(int)(mIconNumbers->GetWidth() / 10), 0, (int)mIconNumbers->GetWidth() / 10, (int)mIconNumbers->GetHeight() };
		destRect = { IconXOffset+(IconPanelIndividualWidth*i)+(int)mIconNumbers->GetWidth() / 10, yIconOffset, (int)mIconNumbers->GetWidth() / 10, 30 };
		mIconNumbers->Render(portionOfSpritesheet, destRect);
	}

	//Draw the quantities on the panels.
	for (int i = 0; i < LEMMINGTYPE_MAX-1; i++) //-1 as LEMMINGTYPE_NORMAL does not have an icon.
	{
		int quantity = GameManager_Lemmings::Instance()->mIconQuantities[i];

		//First digit.
		int w = quantity / 10;
		portionOfSpritesheet = { w*(int)(mIconNumbers->GetWidth()/10), 0, (int)mIconNumbers->GetWidth()/10, (int)mIconNumbers->GetHeight() };
		destRect = { xIconOffset+(IconPanelIndividualWidth*i), yIconOffset, (int)mIconNumbers->GetWidth() / 10, 30 };//(int)mIconNumbers->GetHeight()
		mIconNumbers->Render(portionOfSpritesheet, destRect);

		//Second digit.
		w = quantity % 10;
		portionOfSpritesheet = { w*(int)(mIconNumbers->GetWidth()/10), 0, (int)mIconNumbers->GetWidth() / 10, (int)mIconNumbers->GetHeight() };
		destRect = { xIconOffset + (IconPanelIndividualWidth*i) + (int)mIconNumbers->GetWidth()/10, yIconOffset, (int)mIconNumbers->GetWidth() / 10, 30 };//(int)mIconNumbers->GetHeight()
		mIconNumbers->Render(portionOfSpritesheet, destRect);
	}

	if (GameManager_Lemmings::Instance()->GetSelectedIcon() != LEMMINGTYPE_NORMAL)
	{
		mIconHighlight->Render(Vector2D((IconPanelIndividualWidth * 2) + (IconPanelIndividualWidth*GameManager_Lemmings::Instance()->GetSelectedIcon()), kLemmingsPlayAreaHeight + kScoreHeight));
	}

	//------------------------------------------------------------------------
	//Score panel.
	//------------------------------------------------------------------------
	mScorePanel->Render(Vector2D(0.0f, kLemmingsPlayAreaHeight));

	//Draw the quantities on the score panel.
	//-------------------------------------------------------------------------------------------------------------------------------------------
	//OUT Values
	//First digit.
	int numPos = ObjectManager_Lemmings::Instance()->mCurrentNumberOfLemmings / 10;
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 130, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//Second digit.
	numPos = ObjectManager_Lemmings::Instance()->mCurrentNumberOfLemmings % 10;
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 130 + (int)(mGreenNumbers->GetWidth() / 10), kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };//(int)mIconNumbers->GetHeight()
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//-------------------------------------------------------------------------------------------------------------------------------------------
	//IN Values
	//First digit.
	numPos = GameManager_Lemmings::Instance()->GetNumberOfEscapedLemmings() / 10;
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 335, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//Second digit.
	numPos = GameManager_Lemmings::Instance()->GetNumberOfEscapedLemmings() % 10;
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 335 + (int)(mGreenNumbers->GetWidth() / 10), kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };//(int)mIconNumbers->GetHeight()
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//-------------------------------------------------------------------------------------------------------------------------------------------
	//% Values
	//First digit.
	int qty = (100/kNumberOfLemmings)*GameManager_Lemmings::Instance()->GetNumberOfEscapedLemmings();
	numPos = qty / 10;
	if (numPos == 10)
	{
		numPos = 1;
		portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
		destRect = { 525, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
		mGreenNumbers->Render(portionOfSpritesheet, destRect);
		numPos = 0;
	}
	else
	{
		portionOfSpritesheet = { 0, 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
		destRect = { 525, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
		mGreenNumbers->Render(portionOfSpritesheet, destRect);
	}

	//Second digit.
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 525 + (int)(mGreenNumbers->GetWidth() / 10), kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//Third digit.
	numPos = qty % 10;
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 525 + (int)(mGreenNumbers->GetWidth() / 10)*2, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };//(int)mIconNumbers->GetHeight()
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//-------------------------------------------------------------------------------------------------------------------------------------------
	//Time Values - Seconds
	numPos = (GameManager_Lemmings::Instance()->mLevelTime / 1000) / 10;
	if (numPos >= 10)
	{
		numPos = numPos / 10;
		portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
		destRect = { 875, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
		mGreenNumbers->Render(portionOfSpritesheet, destRect);
		numPos = 0;
	}
	else
	{
		portionOfSpritesheet = { 0, 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
		destRect = { 875, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
		mGreenNumbers->Render(portionOfSpritesheet, destRect);
	}

	//Second digit.
	numPos = (GameManager_Lemmings::Instance()->mLevelTime / 1000) / 10;
	numPos = numPos % 10;
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 875 + (int)(mGreenNumbers->GetWidth() / 10), kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//Third digit.
	numPos = max((GameManager_Lemmings::Instance()->mLevelTime / 1000) % 10, 0);
	portionOfSpritesheet = { numPos*(int)(mGreenNumbers->GetWidth() / 10), 0, (int)mGreenNumbers->GetWidth() / 10, (int)mGreenNumbers->GetHeight() };
	destRect = { 875 + (int)(mGreenNumbers->GetWidth() / 10)*2, kLemmingsPlayAreaHeight, (int)mGreenNumbers->GetWidth() / 10, kScoreHeight };
	mGreenNumbers->Render(portionOfSpritesheet, destRect);

	//-------------------------------------------------------------------------------------------------------------------------------------------
	//Virtual Controller position.
	DrawDebugCircle(VirtualJoypad_Lemmings::Instance()->MousePosition, 2.0f, 255, 255, 255);
	DrawDebugCircle(VirtualJoypad_Lemmings::Instance()->MousePosition, 4.0f, 255, 0, 0);
	DrawDebugCircle(VirtualJoypad_Lemmings::Instance()->MousePosition, 5.0f, 0, 255, 0);
	DrawDebugCircle(VirtualJoypad_Lemmings::Instance()->MousePosition, 6.0f, 0, 0, 255);
}

//--------------------------------------------------------------------------------------------------

void UIManager_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
}

//--------------------------------------------------------------------------------------------------

void UIManager_Lemmings::DrawDebugCircle(Vector2D centrePoint, double radius, int red, int green, int blue)
{
#ifdef DEBUG_LINES_VISIBLE 
	Vector2D polarVec(0.0f, radius);

	float stepSize = 0.02f;
	float _360DegAsRads = (float)DegsToRads(360.0f);

	while (polarVec.x < _360DegAsRads)
	{
		Vector2D polarAsCart(polarVec.y * cosf(polarVec.x), polarVec.y * sinf(polarVec.x));
		Vector2D drawPoint(centrePoint.x + polarAsCart.x, centrePoint.y + polarAsCart.y);

		SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
		SDL_RenderDrawPoint(mRenderer, (int)drawPoint.x, (int)drawPoint.y);

		polarVec.x += stepSize;
	}
#endif
}

//--------------------------------------------------------------------------------------------------