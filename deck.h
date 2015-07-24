#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

struct int_pair{
    int x;
    int y;
    void operator =( const int_pair Pair ){
        x = Pair.x;
        y = Pair.y;
    }
};

class Deck {

    public:

        Deck ();
        void Full ();
        void Shuffle ();
        void Clear();
        Card &Deal ();
        void Add (const Card& card);
        void Delete (const int &index);
        int number () const;
        Card &card (int i);
        void Sort ();
        void Print ();
        Card::Rank special_check (int n) const;
        void operator =(const Deck &deck);

    private:
        vector<Card> cards; // there are 52 cards in each deck
};

int Print_match_first(Deck deck1, Deck deck2, vector<int_pair>);
int Print_match_second(Deck deck, vector<int_pair>);
vector<int_pair> Find_match(Deck deck1, Deck deck2);

#endif // DECK_H
