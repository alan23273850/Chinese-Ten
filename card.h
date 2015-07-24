#ifndef CARD_H
#define CARD_H

#include <string>
#include <windows.h>
#include <iostream>
using namespace std;

void SetColor(int f=7,int b=0);//Used to generate poker-like colors

class Card {

    public:
        enum Suit{ None, Club, Diamond, Heart, Spade };
        enum Rank{ Unknown, Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };

        Card ();
        Card (Suit s, Rank r);
        Suit suit () const;
        Rank rank () const;
        void set_suit (Suit s);
        void set_rank (Rank r);
        char suit_icon () const;
        string suit_name () const;
        string rank_name () const;
        int Points ( int people ) const;
        void operator =(const Card &card);        
        void Print() const;

    private:
        Suit my_suit;
        Rank my_rank;
        static const string Suit_Names[];
        static const string Rank_Names[];
};

#endif // CARD_H
