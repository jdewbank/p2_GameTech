#include "Scoreboard.h"

Scoreboard::Scoreboard(unsigned int num, NetManager* nm, bool s) 
{ 	
	numPlayers = num;
	net = nm;
	server = s;
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
	for(int num = 1; num <= numPlayers; ++num)
		resetScore(num);
}

void Scoreboard::resetScore(int num) 
{ 	//check indexing bounds
	if (num <= numPlayers && num > 0) 
		playerScore[num-1] = 0;
}
    
void Scoreboard::addScore(int points, int num) 
{ 	//check indexing bounds
	if (num <= numPlayers && num > 0) 
		playerScore[num-1] += points;

	if(net)
	{
		if(server)
		{
			const char buf[128] = {0};

			char message = 'a';

			char * c_buf = (char*)buf;
			c_buf[0] = message;

			net->messageClients(PROTOCOL_UDP,buf, sizeof(buf));
		}
	}
}

unsigned int Scoreboard::getScore(int num) 
{ 	//check indexing bounds
	if (num <= numPlayers && num > 0) 
		return playerScore[num-1]; 
	else return 0;
}
