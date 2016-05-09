#pragma once
#include <BWAPI.h>
#include <InformationManager.h>
#include <Random.h>

class MoodManager
{
public:
	class MoodData
	{
	public:
		enum Mood
		{
			// Cheese,
			Rush,
			Agressive,
			Defensive,
			FastExpo,
			Macro
		};
		Mood mood;
	};

	class Points
	{
	public:
		int					zealot;
		int					dragoon;
		int					corsair;
		int					darkTemplar;
	};

	MoodManager();
	~MoodManager();

	MoodData::Mood	getMood								(void);
	Points*					getPoints							(void);
	void						setInformationManager	(InformationManager*);
	void						setMood								(MoodData::Mood);
	void						setRandomMood					(void);
	void						setAnotherRandomMood	(void);
	void						update								(void);

private:
	MoodData						moodData;
	Points							*unitPoints;
	InformationManager	*informationManager;
	Random							*randomMood;
	int									lastFrameCheck;
};
