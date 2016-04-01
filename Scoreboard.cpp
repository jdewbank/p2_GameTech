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
	highScore = 0;
}

void Scoreboard::resetScore(int num) 
{ 	//check indexing bounds
	if (num <= numPlayers && num > 0) 
		playerScore[num-1] = 0;

	if(net)
	{
		if(server)
		{
			const char buf[128] = {0};

			char message = 'r';

			char * c_buf = (char*)buf;
			c_buf[0] = message;
			c_buf[2] = (char) num;

			net->messageClients(PROTOCOL_UDP,buf, sizeof(buf));
		}
	}
}
    
void Scoreboard::addScore(int points, int num) 
{ 	//check indexing bounds
	if (num <= numPlayers && num > 0) 
	{
		playerScore[num-1] += points;
		if (highScore < playerScore[num-1])
			highScore = playerScore[num-1];
	}
	if(net)
	{
		if(server)
		{
			const char buf[128] = {0};

			char message = 'a';

			char * c_buf = (char*)buf;
			c_buf[0] = message;
			c_buf[1] = (char) points;
			c_buf[2] = (char) num;

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

unsigned int Scoreboard::getHighScore() 
{ 	return highScore;
}