#include "Scoreboard.h"

Scoreboard::Scoreboard(unsigned int num) 
{ 	
	numPlayers = num;
	playerScore = new unsigned int[numPlayers];
	resetAll(); 
}

Scoreboard::~Scoreboard() 
{
	if(playerScore) // != null
    	delete[] playerScore; 
}

void Scoreboard::resetAll() 
{ 
	for(int num = 1; num <= numPlayers; num++)
		resetScore(num);
}

void Scoreboard::resetScore(int num) 
{ 	if (num <= numPlayers && num > 0) //check indexing bounds
		playerScore[num-1] = 0;
}
    
void Scoreboard::addScore(int points, int num) 
{ 
	if (num <= numPlayers && num > 0) //check indexing bounds
		playerScore[num-1] += points;
}

unsigned int Scoreboard::getScore(int num) 
{ 	
	if (num <= numPlayers && num > 0) 
		return playerScore[num-1]; 
	else return 0;
}
