#ifndef __Scoreboard_h_
#define __Scoreboard_h_

#include "NetManager.h"

class Scoreboard
{
protected:
	unsigned int numPlayers;
	unsigned int* playerScore;
    unsigned int highScore;
    NetManager* net;
    bool server;

public:
	Scoreboard(unsigned int num, NetManager* net, bool server);
	~Scoreboard();
    void resetAll();
    void resetScore(int num = 1);
    void addScore(int points, int num = 1);
    unsigned int getScore(int num = 1);
    unsigned int getHighScore();
};

#endif // #ifndef __Scoreboard_h_