//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "Landscape_Lemmings.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "Constants_Lemmings.h"
#include "../Commons.h"
#include "GameManager_Lemmings.h"
#include "ObjectManager_Lemmings.h"
#include <assert.h>

//Initialise the instance to null.
Landscape_Lemmings* Landscape_Lemmings::mInstance = NULL;

//--------------------------------------------------------------------------------------------------

Landscape_Lemmings::Landscape_Lemmings()
{

}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::Init(SDL_Renderer* renderer)
{
	mRenderer = renderer;

	//Used for surface manipulation.
	r = new Uint8;
	g = new Uint8;
	b = new Uint8;

	//Get all required textures.
	mReserveGroundTexture = new Texture2D(mRenderer);
	mReserveGroundTexture->LoadFromFile("Images/Lemmings/Landscape/level" + to_string(GameManager_Lemmings::Instance()->mLevelID) + ".bmp");

	mGroundTexture = new Texture2D(mRenderer);
	mGroundTexture->LoadFromFile("Images/Lemmings/Landscape/level" + to_string(GameManager_Lemmings::Instance()->mLevelID) + ".bmp");

	mWaveImage = new Texture2D(mRenderer);
	mWaveImage->LoadFromFile("Images/Lemmings/Landscape/Waves.bmp");

	mDigTexture = new Texture2D(mRenderer);
	mDigTexture->LoadFromFile("Images/Lemmings/Explosions/DigArea.bmp");

	mPunchTexture = new Texture2D(mRenderer);
	mPunchTexture->LoadFromFile("Images/Lemmings/Explosions/PunchArea.bmp");

	mPickAxeTexture = new Texture2D(mRenderer);
	mPickAxeTexture->LoadFromFile("Images/Lemmings/Explosions/PickAxeArea.bmp");

	mExplosionTexture = new Texture2D(mRenderer);
	mExplosionTexture->LoadFromFile("Images/Lemmings/Explosions/Explosion.bmp");

	mBuildPlatformTexture = new Texture2D(mRenderer);
	mBuildPlatformTexture->LoadFromFile("Images/Lemmings/Explosions/BuildPlatform.bmp");

	mStartPosition = Vector2D();
	mEndPosition = Vector2D();

	SetupCollisionMap();
	CreateGroundImage();

	mLandModified = false;
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::RestartLevel()
{
	unsigned char*	groundPixels = (unsigned char*)mGroundTexture->GetSurface()->pixels;
	unsigned char*	reservePixels = (unsigned char*)mReserveGroundTexture->GetSurface()->pixels;
	for (int x = 0; x < kLemmingsPlayAreaWidth; x++)
	{
		for (int y = 0; y < kLemmingsPlayAreaHeight; y++)
		{
			groundPixels[mGroundTexture->GetSurface()->format->BytesPerPixel * (y * mGroundTexture->GetSurface()->w + x)]		= reservePixels[mReserveGroundTexture->GetSurface()->format->BytesPerPixel * (y * mReserveGroundTexture->GetSurface()->w + x)];
			groundPixels[mGroundTexture->GetSurface()->format->BytesPerPixel * (y * mGroundTexture->GetSurface()->w + x) + 1]	= reservePixels[mReserveGroundTexture->GetSurface()->format->BytesPerPixel * (y * mReserveGroundTexture->GetSurface()->w + x) + 1];
			groundPixels[mGroundTexture->GetSurface()->format->BytesPerPixel * (y * mGroundTexture->GetSurface()->w + x) + 2]	= reservePixels[mReserveGroundTexture->GetSurface()->format->BytesPerPixel * (y * mReserveGroundTexture->GetSurface()->w + x) + 2];
		}
	}
	mGroundTexture->RefreshTextureFromSurface();
	UpdateCollisionMap();

	mLandModified = false;
}

//--------------------------------------------------------------------------------------------------

Landscape_Lemmings::~Landscape_Lemmings()
{
	delete mReserveGroundTexture;
	mReserveGroundTexture = NULL;

	delete mGroundTexture;
	mGroundTexture = NULL;

	delete mWaveImage;
	mWaveImage = NULL;

	delete mDigTexture;
	mDigTexture = NULL;

	delete mPunchTexture;
	mPunchTexture = NULL;

	delete mPickAxeTexture;
	mPickAxeTexture = NULL;

	delete mExplosionTexture;
	mExplosionTexture = NULL;

	delete mBuildPlatformTexture;
	mBuildPlatformTexture = NULL;

	for (int i = 0; i < kLemmingsPlayAreaWidth; i++)
	{
		delete mCollisionMap[i];
		mCollisionMap[i] = NULL;
	}
	delete mCollisionMap;
	mCollisionMap = NULL;

	delete r;
	delete g;
	delete b;

	mRenderer = NULL;
	mInstance = NULL;
}

//--------------------------------------------------------------------------------------------------

Landscape_Lemmings* Landscape_Lemmings::Instance()
{
	if (!mInstance)
	{
		mInstance = new Landscape_Lemmings;
	}

	return mInstance;
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::RenderDestructibleLand()
{
	//BLEND required for transparent sea.
//	SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
	mGroundTexture->Render(Vector2D());
}

void Landscape_Lemmings::RenderSea()
{
	int seaLevel = 360;
	SDL_Rect rect;
	rect.x = 0;
	rect.y = seaLevel;
	rect.w = kLemmingsPlayAreaWidth;
	rect.h = kLemmingsPlayAreaHeight - seaLevel;


	//Overlay of blue/green.
	SDL_SetRenderDrawColor(mRenderer, 210, 100, 64, 225);
	SDL_RenderFillRect(mRenderer, &rect);

	int xPos = 0;
	while (xPos < kLemmingsPlayAreaWidth)
	{
		mWaveImage->Render(Vector2D(xPos, seaLevel));
		xPos += mWaveImage->GetWidth();
	}
}
//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
	if(mLandModified)
		RefreshLand();

	mLandModified = false;
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::SetupCollisionMap()
{
	assert(kLemmingsPlayAreaWidth != 0 && kLemmingsPlayAreaHeight != 0);
	mCollisionMap = new LandscapeType_Lemmings*[kLemmingsPlayAreaWidth];
	for (int i = 0; i < kLemmingsPlayAreaWidth; i++)
		mCollisionMap[i] = new LandscapeType_Lemmings[kLemmingsPlayAreaHeight];

	UpdateCollisionMap();
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::RefreshLand()
{
	CreateGroundImage();
}

void Landscape_Lemmings::ModifyLand(int xCentrePos, int yCentrePos, Texture2D & explosionArea)
{
	mLandModified = true;

	float explosionWidth  = (float)explosionArea.GetWidth();
	float explosionHeight = (float)explosionArea.GetHeight();
	float xPos = xCentrePos - explosionWidth*0.5f;
	float yPos = yCentrePos - explosionHeight*0.5f;

	SDL_Surface* surfaceExplosion = explosionArea.GetSurface();

	for (int x = 0; x < explosionWidth; x++)
	{
		for (int y = 0; y < explosionHeight; y++)
		{
			if (xPos + x >= 0 && xPos + x < kLemmingsPlayAreaWidth &&
				yPos + y >= 0 && yPos + y < kLemmingsPlayAreaHeight)
			{
				Uint32 pixel = GetPixel(surfaceExplosion, x, y);
				SDL_GetRGB(pixel, surfaceExplosion->format, r, g, b);

				//Exploded area needs to be BLACK.
				if (*r == 0 && *g == 0 && *b == 0)
				{
					mCollisionMap[(int)xPos + x][(int)yPos + y] = LANDSCAPETYPE_EMPTY;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::UpdateCollisionMap()
{
	SDL_Surface* surface = mGroundTexture->GetSurface();

	for (int x = 0; x < kLemmingsPlayAreaWidth; x++)
	{
		for (int y = 0; y < kLemmingsPlayAreaHeight; y++)
		{
			Uint32 pixel = GetPixel(surface, x, y);
			SDL_GetRGB(pixel, surface->format, r, g, b);

			//Empty cells need to be BLACK, RED (End position) or GREEN (Start position).
			if (*r == 0 && *g == 0 && *b == 0)
				mCollisionMap[x][y] = LANDSCAPETYPE_EMPTY;
			else if (*r == 0 && *g == 255 && *b == 0)
			{
				mCollisionMap[x][y] = LANDSCAPETYPE_EMPTY;
				mStartPosition = Vector2D(x, y);
			}
			else if (*r == 255 && *g == 0 && *b == 0)
			{
				mCollisionMap[x][y] = LANDSCAPETYPE_EMPTY;
				mEndPosition = Vector2D(x, y);
			}
			else
				mCollisionMap[x][y] = LANDSCAPETYPE_GROUND;
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::CreateGroundImage()
{
	//Ground image.
	SDL_Surface*		surface = mGroundTexture->GetSurface();
	SDL_PixelFormat*	format = surface->format;
	unsigned char*		pixels = (unsigned char*)surface->pixels;

	for (int x = 0; x < kLemmingsPlayAreaWidth; x++)
	{
		for (int y = 0; y < kLemmingsPlayAreaHeight; y++)
		{
			if (mCollisionMap[x][y] == LANDSCAPETYPE_EMPTY)
			{
				pixels[format->BytesPerPixel * (y * surface->w + x)] = 0xFF;		//Blue
				pixels[format->BytesPerPixel * (y * surface->w + x) + 1] = 0xFF;	//Green
				pixels[format->BytesPerPixel * (y * surface->w + x) + 2] = 0;		//Red
			}
		}
	}

	mGroundTexture->RefreshTextureFromSurface();
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::DigLand(int xCentrePos, int yCentrePos)
{
	ModifyLand(xCentrePos, yCentrePos, *mDigTexture);
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::PunchLand(int xCentrePos, int yCentrePos)
{
	ModifyLand(xCentrePos, yCentrePos, *mPunchTexture);
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::PickAxeLand(int xCentrePos, int yCentrePos)
{
	ModifyLand(xCentrePos, yCentrePos, *mPickAxeTexture);
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::RemoveLandForExplosion(int xCentrePos, int yCentrePos)
{
	ModifyLand(xCentrePos, yCentrePos, *mExplosionTexture);
	ObjectManager_Lemmings::Instance()->ExplosionAt(Vector2D(xCentrePos, yCentrePos));
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::AddLandForPlatform(int xPos, int yPos)
{
	//This is instant, unlike explosions.
	int platformWidth  = mBuildPlatformTexture->GetWidth();
	int platformHeight = mBuildPlatformTexture->GetHeight();

	//Resultant image.
	SDL_Surface*		surface = mGroundTexture->GetSurface();
	SDL_PixelFormat*	format = surface->format;
	unsigned char*		pixels = (unsigned char*)surface->pixels;

	Uint8*	r = new Uint8;
	Uint8*	g = new Uint8;
	Uint8*	b = new Uint8;

	SDL_Surface* surfaceExplosion = mBuildPlatformTexture->GetSurface();

	for (int x = 0; x < platformWidth; x++)
	{
		for (int y = 0; y < platformHeight; y++)
		{
			if (xPos + x >= 0 && xPos + x < kLemmingsPlayAreaWidth &&
				yPos + y >= 0 && yPos + y < kLemmingsPlayAreaHeight)
			{
				Uint32 pixel = GetPixel(surfaceExplosion, x, y);
				SDL_GetRGB(pixel, surfaceExplosion->format, r, g, b);

				//Exploded area needs to be BLACK.
				if (*r == 0 && *g == 0 && *b == 0)
				{
					mCollisionMap[xPos + x][yPos + y] = LANDSCAPETYPE_EMPTY;

					//Transparency.
					pixels[format->BytesPerPixel * ((yPos + y) * surface->w + (xPos + x))] = 0xFF;		//Blue
					pixels[format->BytesPerPixel * ((yPos + y) * surface->w + (xPos + x)) + 1] = 0xFF;	//Green
					pixels[format->BytesPerPixel * ((yPos + y) * surface->w + (xPos + x)) + 2] = 0;		//Red
				}
				else
				{
					mCollisionMap[xPos + x][yPos + y] = LANDSCAPETYPE_GROUND;

					//Step.
					pixels[format->BytesPerPixel * ((yPos + y) * surface->w + (xPos + x))] = 87;		//Blue
					pixels[format->BytesPerPixel * ((yPos + y) * surface->w + (xPos + x)) + 1] = 122;	//Green
					pixels[format->BytesPerPixel * ((yPos + y) * surface->w + (xPos + x)) + 2] = 185;	//Red
				}
			}
		}
	}

	mGroundTexture->RefreshTextureFromSurface();

	delete r;
	delete g;
	delete b;
}

//--------------------------------------------------------------------------------------------------

Uint32 Landscape_Lemmings::GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to retrieve
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       // shouldn't happen, but avoids warnings
	}
}

//--------------------------------------------------------------------------------------------------

void Landscape_Lemmings::SetPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	Uint32* pixels = (Uint32*)surface->pixels;
	//Uint32* pixel = pixels + y*surface->pitch/4 + x;
	//*pixel = SDL_MapRGB(surface->format, r, g, b);
	pixels[y*surface->w + x] = r;
	pixels[y*surface->w + x + 1] = g;
	pixels[y*surface->w + x + 2] = b;
}

//--------------------------------------------------------------------------------------------------

LandscapeType_Lemmings Landscape_Lemmings::GetGroundTypeAt(Vector2D position)
{
	return GetGroundTypeAt((int)position.x, (int)position.y);
}

LandscapeType_Lemmings Landscape_Lemmings::GetGroundTypeAt(int x, int y)
{
	assert(x >= 0 && x < kLemmingsPlayAreaWidth && y >= 0 && y < kLemmingsPlayAreaHeight);
		
	return mCollisionMap[x][y]; 
}

//--------------------------------------------------------------------------------------------------
