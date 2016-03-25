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
    void resetScore(int num);
    void addScore(int points, int num);
    unsigned int getScore(int num);
};

#endif // #ifndef __Scoreboard_h_