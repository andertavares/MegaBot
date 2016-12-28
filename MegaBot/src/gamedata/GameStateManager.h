#pragma once
#include <map>
#include "GameState.h"

class GameStateManager {
	
	GameStateManager(void);
	static GameStateManager* instance;

	map<int,GameState> gameStates;

	/** How frequently should I save game states? */
	int frequency;

public:
	
	/** Returns the instance of this singleton */
	static GameStateManager* getInstance();

	/** To be called every frame */
	void onFrame();

	/** Records the game state of this frame */
	void recordState();

	/** Sets the interval between saving two states */
	void setFrequency(int freq);

	~GameStateManager(void);
};

