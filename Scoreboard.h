#ifndef __Scoreboard_h_
#define __Scoreboard_h_

class Scoreboard
{
protected:
	unsigned int bestScore;
	unsigned int newScore;
public:
	Scoreboard();
	~Scoreboard();
    void resetAll();
    void resetScore();
    void addScore(int);
    unsigned int getScore() { return newScore; };
    unsigned int getBest()  { return bestScore; };
};

#endif // #ifndef __Scoreboard_h_