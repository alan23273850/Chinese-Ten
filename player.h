#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

class Player {
    public:
        //General Part
        Player (wstring Name = L"PLAYER");
        void GetCardToHand (const Card &card);
        void GetCardToPile (const Card &card);
        Deck GetHand () const;
        Deck GetPile () const;
        int CaptureCard (int ToCapture_index, int layout_index, Deck &ToCapture, Deck &layout);
        void Play (int people, Deck &Main, Deck &layout, Card card);
        void SetName (wstring Name);
        wstring GetName () const;
        int Points (int people);
        void operator =(const Player &player);

        //Computer Part
        void EnableComputer();
        void DisableComputer();
        bool ComputerOn() const;
        bool ComputerOff() const;

private:
        Deck Hand;
        Deck Pile;
        wstring name; // Player's name
        bool computer;

        //Subroutines for playing
        inline void print_whos_turn ();
        inline void print_layout_and_own_cards (Deck &layout);
        inline void first_stage_pairing (int people, Deck &layout);
        inline void second_stage_prepare (Deck &Main);
        inline void second_stage_special_case_check (Deck &Main, Deck &layout, Card card);
        inline void second_stage_pairing (int people, Deck &Main, Deck &layout);
        inline void finish_this_turn ();
        inline int choose_card_X (int com_choice);
        inline int choose_card_Y (Deck &layout, int com_choice);
};

#endif // PLAYER_H
