#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

class Player {

    public:

        //General Part
        Player ( string Name = "PLAYER" );
        void GetCardToHand ( const Card &card );
        void GetCardToPile ( const Card &card );
        Deck GetHand () const;
        Deck GetPile () const;
        int CaptureCard ( int ToCapture_index, int layout_index, Deck &ToCapture , Deck &layout);
        int Play(int people, Deck &Main, Deck &layout, Card card );
        void SetName( string Name );
        string GetName( ) const;
        int Points ( int people );
        void operator =(const Player &player);

        //Computer Part
        void EnableComputer();
        void DisableComputer();
        bool ComputerOn() const;
        bool ComputerOff() const;
        void PressAnyKey() const;

private:
        Deck Hand;
        Deck Pile;
        string name; // Player's name
        bool computer;
};

#endif // PLAYER_H
