#pragma once
#define GAME

#ifdef GAME
#define GAMESTATE DeadlyLibrary
#define GAMESTATE_H "..\gameState\dL.h"
#else
#define GAMESTATE Planets
#define GAMESTATE_H "..\gameState\Planets.h"
#endif