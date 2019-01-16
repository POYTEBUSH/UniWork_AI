//------------------------------------------------------------------------
//  Author: Paul Roberts 2015
//------------------------------------------------------------------------

#include "Texture2D.h"
#include <iostream>
#include <SDL_image.h>

using namespace::std;
//--------------------------------------------------------------------------------------------------

Texture2D::Texture2D(SDL_Renderer* renderer)
{
	//Initialise member variables.
	mRenderer = renderer;
	mTexture  = NULL;
	mWidth    = 0;
	mHeight   = 0;
}

//--------------------------------------------------------------------------------------------------

Texture2D::~Texture2D()
{
	//Free up the memory.
	Free();

	mRenderer = NULL;
}

//--------------------------------------------------------------------------------------------------

bool Texture2D::LoadFromFile( string path )
{
	//Remove the memory used for a previous texture.
	Free();

	mTexture = NULL;

	//Load the image.
	mSurface = IMG_Load( path.c_str() );
	if(mSurface != NULL)
	{
		//Colour key the image - The colour to be transparent.
		SDL_SetColorKey(mSurface, SDL_TRUE, SDL_MapRGB(mSurface->format, 0, 0xFF, 0xFF));

		//Create the texture from the pixels on the surface.
		mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
		if(mTexture != NULL)
		{
			//Set the dimensions.
			mWidth  = mSurface->w;
			mHeight = mSurface->h;
		}
		else
		{
			cout << "Unable to create texture from surface. Error: " << IMG_GetError() << endl;
		}

		//Remove the loaded surface now that we have the texture.
		//SDL_FreeSurface(mSurface);
	}
	else
	{
		cout << "Image not loaded. Error: " << SDL_GetError() << endl;
	}

	//Return whether the process was successful.
	return mTexture != NULL;
}

//--------------------------------------------------------------------------------------------------

void Texture2D::RefreshTextureFromSurface()
{
	SDL_DestroyTexture(mTexture);
	mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
	if (mTexture != NULL)
	{
		//Set the dimensions.
		mWidth = mSurface->w;
		mHeight = mSurface->h;
	}
}

//--------------------------------------------------------------------------------------------------

void Texture2D::Free()
{
	//Check if the texture exists before removing it.
	if(mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth   = 0;
		mHeight  = 0;
	}
}

//--------------------------------------------------------------------------------------------------

void Texture2D::Render(double x, double y, SDL_RendererFlip flip, double angle, int alpha)
{
	//Set where to render the texture.
	SDL_Rect renderLocation = {(int)x, (int)y, mWidth, mHeight};

	//Set the opacity.
	SDL_SetTextureAlphaMod(mTexture, alpha);

	//Render to screen.
	SDL_RenderCopyEx(mRenderer, mTexture, NULL, &renderLocation, angle, NULL, flip);
}

//--------------------------------------------------------------------------------------------------

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle, int alpha)
{
	Render(newPosition.x, newPosition.y, flip, angle, alpha);
}

//--------------------------------------------------------------------------------------------------

void Texture2D::Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle, int alpha)
{
	//Set the opacity.
	SDL_SetTextureAlphaMod(mTexture, alpha);

	//Render to screen.
	SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &destRect, angle, NULL, flip);
}

//--------------------------------------------------------------------------------------------------