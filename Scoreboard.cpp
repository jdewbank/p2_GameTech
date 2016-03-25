#include "Scoreboard.h"

#define MAX 2

Scoreboard::Scoreboard() 
{ 	
	playerScore = new unsigned int[MAX];
	resetAll(); 
}

Scoreboard::~Scoreboard() 
{
	if(playerScore) // != null
    	delete[] playerScore; 
}

void Scoreboard::resetAll() 
{ 
	for(int num = 1; num <= MAX; num++)
		resetScore(num);
}

void Scoreboard::resetScore(int num) 
{ 	if (num <= MAX && num > 0) //check indexing bounds
		playerScore[num-1] = 0;
}
    
void Scoreboard::addScore(int points, int num) 
{ 
	if (num <= MAX && num > 0) //check indexing bounds
		playerScore[num-1] += points;
}

unsigned int Scoreboard::getScore(int num) 
{ 	
	if (num <= MAX && num > 0) 
		return playerScore[num-1]; 
	else return 0;
}
