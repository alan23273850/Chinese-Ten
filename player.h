#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h" // Deck

class Player {
    public:
        // General Part
        Player(string name = "PLAYER");
        Deck hand() { return my_hand; }
        Deck captured() { return my_captured; }
        string name() { return my_name; }
        void give_name(string name) { my_name = name; }
        void get_card_to_my_hand(Card card) { my_hand.add(card); }
        void get_card_to_my_captured(Card card) { my_captured.add(card); }
        void operator=(const Player &player);
        void play(Card::Rank special_rank);
        int points(int people);

        // Computer Part
        void enable_computer() { computer = true; }
        bool is_computer() { return computer; }

    private:
        Deck my_hand;
        Deck my_captured;
        string my_name;
        bool computer;

        // Subroutines for playing
        void first_stage_pairing();
        void second_stage_pairing();
        bool second_stage_special_case_check(Card::Rank special_rank);
};

#endif // PLAYER_H
