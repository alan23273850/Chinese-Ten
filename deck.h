#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

class Deck {
    public:
        Deck ();
        void Full ();
        void Shuffle ();
        void Clear();
        Card &Deal ();
        void Add (const Card& card);
        void Delete (const int &index);
        size_t number () const;
        Card &card (size_t i);
        void Sort ();
        void Print ();
        Card::Rank special_check (int n) const;
        void operator =(const Deck &deck);

    private:
        vector<Card> cards; // there are 52 cards in each deck
};

bool Print_match_first(Deck deck1, Deck deck2, vector<pair<int, int>>);
int Print_match_second(Deck deck, vector<pair<int, int>>);
vector<pair<int, int>> Find_match(Deck deck1, Deck deck2);

#endif // DECK_H
