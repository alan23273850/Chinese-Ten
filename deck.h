#ifndef DECK_H
#define DECK_H

#include <vector> // vector
#include "card.h" // Card

class Deck {
    public:
        Deck() {} // Constructor, create an empty deck.
        void add(Card card) { cards.push_back(card); }
        int size() const { return cards.size(); } // const: find_match
        Card deal() { Card card = cards.back(); cards.pop_back(); return card; }
        Card deal(int i) { Card card = cards.at(i); cards.erase(cards.begin() + i); return card; }
        Card card(int i) const { return cards.at(i); } // const: find_match
        Card top() { return cards.back(); }
        void fill_and_shuffle();
        void sort();
        void print();
        void operator=(const Deck &deck) { cards = deck.cards; }
        Card::Rank special_check(int n);

    private:
        vector<Card> cards; // there are 52 cards in each deck
};

vector<pair<pair<int, Card>, pair<int, Card>>> find_match(const Deck &deck1, const Deck &deck2);
vector<pair<pair<int, Card>, pair<int, Card>>> find_match(const Card &card, const Deck &deck2);
bool print_match_first(const vector<pair<pair<int, Card>, pair<int, Card>>> &pairs);
bool print_match_second(const vector<pair<pair<int, Card>, pair<int, Card>>> &pairs);

#endif // DECK_H
