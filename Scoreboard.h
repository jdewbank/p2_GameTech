#ifndef __Scoreboard_h_
#define __Scoreboard_h_

class Scoreboard
{
protected:
	unsigned int numPlayers;
	unsigned int* playerScore;

public:
	Scoreboard(unsigned int num = 2);
	~Scoreboard();
    void resetAll();
    void resetScore(int num = 1);
    void addScore(int points, int num = 1);
    unsigned int getScore(int num = 1);
};

#endif // #ifndef __Scoreboard_h_