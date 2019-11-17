#ifndef CARD_H
#define CARD_H

#include <string> // string

using namespace std; // string

class Card {
    public:
        enum class Suit {None, Spade, Heart, Diamond, Club};
        enum class Rank {Unknown, Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King};

        Card() { my_suit = Suit::None; my_rank = Rank::Unknown; }
        Card(Suit suit, Rank rank) { my_suit = suit; my_rank = rank; }
        Suit suit() const { return my_suit; } // const: operator=
        Rank rank() const { return my_rank; } // const: operator=
        string rank_name() const { return rank_names[(int)my_rank]; } // const: print
        string suit_name() { return suit_names[(int)my_suit]; }
        string suit_icon() const; // const: print
        int points(int people);
        void print() const; // const: print_match_first, print_match_second
        void set_rank(Rank rank) { my_rank = rank; }
        void operator=(const Card &card) { my_suit = card.suit(); my_rank = card.rank(); }
        static string rank_name(Rank rank) { return rank_names[(int)rank]; }
        static bool cmp_by_suit(const Card &s1, const Card &s2) { return s1.my_suit > s2.my_suit; }
        static bool cmp_by_rank(const Card &s1, const Card &s2) { return s1.my_rank < s2.my_rank; }
        static bool can_capture(Card card1, Card card2);

    private:
        Suit my_suit;
        Rank my_rank;
        static const string suit_names[];
        static const string rank_names[];
};

#endif // CARD_H
