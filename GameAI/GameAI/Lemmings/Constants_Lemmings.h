//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#pragma once

//Screen dimensions.
const int	kLemmingsPlayAreaWidth		 = 1000;		//In pixels.
const int	kLemmingsPlayAreaHeight		 = 400;
const int	kScoreHeight				 = 57;
const int	kIconHeight					 = 96;		//This needs adding on to the screen height to accomodate the icons.
const int	IconPanelIndividualWidth	 = 64;
const int	IconXOffset					 = 16;
const int	IconYOffset					 = 4;
const int	kLemmingsScreenWidth		 = 1000;
const int	kLemmingsScreenHeight		 = kLemmingsPlayAreaHeight+kIconHeight+kScoreHeight;

const int	kNumberOfLemmings		     = 10;
const float kInitialSpawnTime			 = 2500.0f;
const float kMaxSpawnTime				 = 5000.0f;
const float kMinSpawnTime				 = 100.0f;
const float kSpawnTimeIncrement			 = 100.0f;

const float kGravity_Lemmings			 = 0.007f;
const float kParachuteAntiGravityForce   = -0.005f;
const float kLemmingFallSpeed			 = 3.0f;
const float kLemmingDeathFallSpeed		 = 5.59f;

const float kMaxXVelocity				 = 1.5f; 
const float kMaxYVelocity				 = 5.8f;

const float kExplosionRadius			 = 25.0f;
const float kExplosionStrength			 = 0.4f;
const int	kDetonationDuration			 = 5000;
const int	kDetonationDurationInSeconds = 5;

const float kAnimationDelay_Lemmings	 = 150.0f;
const float kLemmingWalkSpeed			 = 0.04f;
const float kLemmingClimbSpeed			 = -0.01f;
const float kLemmingClimbExitSpeed		 = -0.025f;
const float kLemmingJumpSpeed			 = 0.08f;
const float kLemmingPunchXMovement		 = 0.5f;
const float kLemmingPickAxeXMovement	 = 0.5f;

const int kBuildDuration				 = 20000;