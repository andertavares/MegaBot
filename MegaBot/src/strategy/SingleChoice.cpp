#include "SingleChoice.h"


SingleChoice::SingleChoice(string choice) {
	
	name = "SingleChoice(" + choice + ")";
	
	if (portfolio.find(choice) == portfolio.end()) { //choice not found 
		Logging::getInstance()->log(
			"Choice %s not found in portfolio. Defaulting to Skynet", choice.c_str()
		);
		choiceName = SKYNET;
		return;
	}

	//everything OK, accept choice
	choiceName = choice;
}


SingleChoice::~SingleChoice(void) {
}

void SingleChoice::onStart(){
	currentStrategy = portfolio[choiceName];
	Logging::getInstance()->log("SingleChoice initializing %s", choiceName.c_str());

	currentStrategy->onStart();
}
