#include "Scoreboard.h"

Scoreboard::Scoreboard() 
{ 
	resetAll(); 
}

Scoreboard::~Scoreboard() {}


void Scoreboard::resetAll() 
{ 
	bestScore = 0;
	newScore = 0;
}

void Scoreboard::resetScore() 
{ 
	newScore = 0;
}
    
void Scoreboard::addScore(int points) 
{ 
	newScore += points;
	if (bestScore < newScore)
		bestScore = newScore;
}
