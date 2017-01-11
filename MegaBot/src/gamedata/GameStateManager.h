#pragma once
#include <map>
#include "GameState.h"
#include "SpottedObject.h"

class GameStateManager {
	
	GameStateManager(void);
	static GameStateManager* instance;

	map<int,GameState> gameStates;

	/** How frequently should I save game states? */
	int frequency;

	/** Records the game state of this frame */
	void recordState();

	/** Path to file where state information will be written */
	string stateDumpFile;

public:
	
	/** Returns the instance of this singleton */
	static GameStateManager* getInstance();

	/** To be called every frame */
	void onFrame();

	

	/** Sets the interval between saving two states */
	void setFrequency(int freq);

	/** Takes into account that a unit has been destroyed */
	void unitDestroyed(BWAPI::Unit* unit);

	~GameStateManager(void);
};

