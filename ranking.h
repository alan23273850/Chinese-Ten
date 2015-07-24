#ifndef RANKING_H
#define RANKING_H
#include "player.h"

struct Score{
    string name;
    int score;
    void operator =(const Score right){
        name = right.name;
        score = right.score;
    }
};

class Ranking {
    public:
        Ranking();
        Score GetPlayer(int i);
        int Recording(int people );
        void Push_back(Score player);
        void Clear();
        int FindTop(int play_persons, unsigned int top_persons);
        int FindByName(int play_persons, string search);
        void Pop_back();
        void SortByScore();
        void SortByName();
        void Print(unsigned int n=0);

    private:
        vector<Score> players;
};

#endif // RANKING_H
