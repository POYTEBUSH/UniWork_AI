//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

//Screen dimensions.
const int kSnakeScoreHeight		= 40;
const int kSnakePlayAreaWidth	= 500;		//In pixels.
const int kSnakePlayAreaHeight  = 500;
const int kSnakeScreenWidth		= 500;
const int kSnakeScreenHeight	= kSnakePlayAreaHeight+kSnakeScoreHeight; //The first 40 pixelks are the score.

//Tile dimensions.
const int kSnakeTileDimensions	= 10;

//Update
const int kUpdateDelay			= 300;
const int kMinUpdateDelay		= 50;
const int kDelayReductionAmount	= 10;
const int kTimePerLevel			= 60000;

//Evolutionary Neural Network.
const int kNumberOfNNSnakes				= 50;// 10;
const int kNumberOfInputNodes_Snake		= 12;
const int kNumberOfHiddenNodes_Snake	= 8;
const int kNumberOfOutputNodes_Snake	= 4;
const int kNumberOfWeights_Snake		= (kNumberOfInputNodes_Snake*kNumberOfHiddenNodes_Snake)+(kNumberOfHiddenNodes_Snake*kNumberOfOutputNodes_Snake);

const int kCrossoverRate_Snake	= 3000;						//This is in a range of ints up to 10000 making it 30%
const int kMutationRate_Snake	= 500;						//This is in a range of ints up to 10000 making it 5%

//Input modifiers for Neural Network.
const double kScoreModifierWeight = 100.0;
const double kPickupMultiplierWeight = 1.0;
const double kCollisionMultiplierWeight = 1.0;

//------------------------------------------------------------------------
#endif //SNAKE_CONSTANTS_H