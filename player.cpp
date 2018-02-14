#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "player.h"
#include "color.h"

using namespace std;

const int TIME_CONSTANT=800;

int GetKey () { // Used to convert input char into the proper integer.
    char key = (char)_getche();
    if( '0'<=key && key<='9' )
        return key-'0';
    else if( 'a'<=key && key<='z' )
        return key-'a'+10;
    else if( 'A'<=key && key<='Z' )
        return key-'A'+10;
    else
        return -1;
}

wchar_t int_to_char (int n);

Player::Player (wstring Name) { // constructor
    name = Name; // Note that the default name is "Player" defined in "player.h"
    Deck deck;
    Hand = deck;
    Pile = deck;
    computer = false;
}

void Player::GetCardToHand (const Card &card) { // Add a card to the player's Hand deck.
    Hand.Add(card);
}

void Player::GetCardToPile (const Card &card) { // Add a card to the player's Pile deck.
    Pile.Add(card);
}

Deck Player::GetHand () const {
    return Hand;
}

Deck Player::GetPile () const {
    return Pile;
}

int Player::CaptureCard (int ToCapture_index, int layout_index, Deck &ToCapture, Deck &layout){
    //Use the card in "ToCapture" to capture the card in "layout."
    //Indexes help us to get the card we want.

    if( !(0<=ToCapture_index && ToCapture_index<(int)ToCapture.number() && 0<=layout_index && layout_index<(int)layout.number()) )
        return -1;

    int legal = 0;
    //Check if the pair is legal.
    if( ToCapture.card(ToCapture_index).rank() + layout.card(layout_index).rank() == 10 )
        legal = 1;
    else if( ToCapture.card(ToCapture_index).rank() == layout.card(layout_index).rank() ){
        switch(static_cast<int>(ToCapture.card(ToCapture_index).rank())){
            case 10:
            case 11:
            case 12:
            case 13:
                legal = 1;
                break;
            default:
                legal = 0;
                break;
        }
    }
    else
        legal = 0;

    //If legal, move those cards to proper positions.
    if( legal == 1 ){
        Pile.Add(ToCapture.card(ToCapture_index));
        Pile.Add(layout.card(layout_index));
        ToCapture.Delete(ToCapture_index);
        layout.Delete(layout_index);
    }

    return legal;
}

inline void Player::print_whos_turn () {
    system("cls");
    wcout << L"=====================It\'s " << name << L"\'s turn.=====================\n\n";
}

inline void Player::print_layout_and_own_cards (Deck &layout) {
    wcout << L"Layout:\n";
    layout.Print();
    wcout << L"\nCards in hand:\n";
    Hand.Print();
}

inline int Player::choose_card_X (int com_choice) {
    int X;
    while (true) {
        wcout << L"X = ";
        if( ComputerOn()==true ){ //Computer Operation
            wcout << int_to_char(com_choice);
            Sleep(TIME_CONSTANT);
            X = com_choice;
        }
        else
            X = GetKey();
        wcout << endl;
        if( X<0 || X>(int)Hand.number()-1 ) //Check out of range
            wcout << L"\nYour input index is out of range!!\n";
        else
            break;
    }
    return X;
}

inline int Player::choose_card_Y (Deck &layout, int com_choice) {
    int Y;
    while (true) {
        wcout << L"Y = ";
        if( ComputerOn()==true ){ //Computer Operation
            wcout << int_to_char(com_choice);
            Sleep(TIME_CONSTANT);
            Y = com_choice;
        }
        else
            Y = GetKey();
        if( Y<0 || Y>(int)layout.number()-1 ) //Check out of range
            wcout << L"\nYour input index is out of range!!\n";
        else
            break;
    }
    return Y;
}

inline void Player::first_stage_pairing (int people, Deck &layout) {
    //Variables
    vector<pair<int,int>> Pair;
    int menu, X, Y, index_x=0, index_y=0;

    //Find all available matchings.
    for(unsigned int i=0; i<Find_match(Hand,layout).size(); i++)
        Pair.push_back( Find_match(Hand,layout).at(i) );

    //Show auto quick search.
    if( Print_match_first(Hand,layout,Pair) )
        menu = 1; // matches exist
    else
        menu = 2; // matches do not exist

    //Menu
    switch (menu) {
        while (true) {
            case 1:
                while (true) {
                    //Print prompt message
                    wcout << L"\nI want to use my <X>th card to capture the <Y>th card.\n";

                    //Computer Operation: Choose its own answer by heuristic first.
                    if( ComputerOn() ){
                        int maxVal = static_cast<int>(Hand.card(Pair.at(0).first).Points(people))+static_cast<int>(layout.card(Pair.at(0).second).Points(people));
                        index_x = Pair.at(0).first;
                        index_y = Pair.at(0).second;
                        for(unsigned int i=0; i<Pair.size(); i++){
                            if( static_cast<int>(Hand.card(Pair.at(i).first).Points(people))+static_cast<int>(layout.card(Pair.at(i).second).Points(people)) > maxVal ){
                                maxVal = static_cast<int>(Hand.card(Pair.at(i).first).Points(people))+static_cast<int>(layout.card(Pair.at(i).second).Points(people));
                                index_x = Pair.at(i).first;
                                index_y = Pair.at(i).second;
                            }
                        }
                    }

                    //Choose which cards (X) to capture and to be captured.
                    X = choose_card_X(index_x);

                    //Choose which cards (Y) to capture and to be captured.
                    Y = choose_card_Y(layout, index_y);

                    //Capture and print the message.
                    if( CaptureCard(X, Y, Hand, layout) == 1 ){
                        system("cls");
                        wcout << L"Layout:\n";
                        layout.Print();
                        wcout << L"\nCards in hand:\n";
                        Hand.Print();
                        wcout << L"\nNumber of captured cards: " << Pile.number() << endl;
                        break;
                    }
                    else
                        wcout << L"\nSorry, this action is invalid.\n";
                }
                break;
            case 2:
                while (true) {
                    //Print prompt message.
                    wcout << L"\nNo cards are to be captured.\n";
                    wcout << L"I want to discard my <X>th card. ";
                    wcout << L"X = ";

                    //Choose which card to be discarded to the layout.
                    if( ComputerOn()==true ){ //Computer operation
                        int minVal = Hand.card(0).Points(people);
                        int index = 0;
                        for(int i=0; i<(int)Hand.number(); i++){
                            if( Hand.card(i).Points(people)<minVal ){
                                minVal = Hand.card(i).Points(people);
                                index = i;
                            }
                        }
                        X = index;
                        wcout << int_to_char(X);
                        Sleep(TIME_CONSTANT);
                    }
                    else
                        X = GetKey();
                    wcout << endl;
                    if( X<0 || X>(int)Hand.number()-1 ){
                        wcout << L"\nYour input index is out of range!!\n";
                        continue; // redo this action again.
                    }

                    //Discard.
                    layout.Add(Hand.card(X));
                    Hand.Delete(X);

                    //Print.
                    system("cls");
                    wcout << L"Layout:\n";
                    layout.Print();
                    wcout << L"\nCards in hand:\n";
                    Hand.Print();
                    break;
                }
                break;
            default:
                continue;
        }
    }
}

inline void Player::second_stage_prepare (Deck &Main) {
    //Second stage
    wcout << L"\nNow we are going to flip a card from the deck.\n";

    //Simulate the flipping process.
    wcout << L"Waiting";
    for(int i=6; i>=0; i--){
        Sleep(130*i);
        wcout << '.';
    }

    //Print the flipped card.
    wcout << L"\nThe flipped card is: ";
    Main.card(Main.number()-1).Print();
    wcout << endl;
}

inline void Player::second_stage_special_case_check (Deck &Main, Deck &layout, Card card) {
    if( Main.card(Main.number()-1).rank_name()==card.rank_name() ){
        wcout << L"\nNow the layout contains three cards of rank \"" << card.rank_name() << ".\"" << endl << endl;
        wcout << L"This is the special case." << endl;
        wcout << L"You are going to capture the three cards on the layout with your ";
        if( Main.card(Main.number()-1).suit_name() == L"Diamond" || Main.card(Main.number()-1).suit_name() == L"Heart" )
            ColorHandler::SetColor(4,7);
        else
            ColorHandler::SetColor(0,7);
        wcout << Main.card(Main.number()-1).suit_icon() << Main.card(Main.number()-1).rank_name();
        ColorHandler::SetBackground();
        wcout << L".\n\n";

        layout.Sort();

        //Capture
        int count = 0;
        for(int i=0; i<(int)layout.number() && count<3; i++){
            if( layout.card(i).rank_name()==card.rank_name() ){
                GetCardToPile(layout.card(i));
                layout.Delete(i);
                count++;
                i--;
            }
        }
        GetCardToPile(Main.Deal());

        wcout << L"Capturing";
        for(int i=5; i>=0; i--){
            Sleep(500);
            wcout <<'.';
        }
        wcout << endl << endl;
        wcout << L"Now you have captured all cards of rank \"" << card.rank_name() << L".\"\n";
        wcout << L"\nNumber of your all captured cards: " << Pile.number() << endl;
        wcout << L"\nYour turn is going to finish.\n";
        wcout << L"\nPress any key to continue...";
        cin.sync();

        //Computer operation
        if( ComputerOn()==true )
            Sleep(TIME_CONSTANT);
        else
            _getch();
        return;
    }
}

inline void Player::second_stage_pairing (int people, Deck &Main, Deck &layout) {
    // Variables
    vector<pair<int,int>> Pair;
    Deck temp_deck;
    int menu, X;

    //Show auto quick search.
    temp_deck.Add(Main.card(Main.number()-1));
    if( Print_match_second(layout,Find_match(temp_deck,layout))==1 )
        menu = 1;
    else
        menu = 2;

    //Menu
    switch(menu){
        while (true) {
            case 1:
                while (true) {
                    //Choose which cards to capture and to be captured.
                    wcout << L"\nI want to capture the <X>th card. ";
                    wcout << L"X = ";

                    //Computer operation
                    temp_deck.Clear();
                    Pair.clear();
                    temp_deck.Add(Main.card(Main.number()-1));
                    for(unsigned int i=0; i<Find_match(temp_deck,layout).size(); i++)
                        Pair.push_back( Find_match(temp_deck,layout).at(i) );
                    if( ComputerOn()==true ){
                        int maxVal = static_cast<int>(Main.card(Main.number()-1).Points(people))+static_cast<int>(layout.card(Pair.at(0).second).Points(people));
                        int idx = Pair.at(0).second;
                        for(unsigned int i=0; i<Pair.size(); i++){
                            if( static_cast<int>(Main.card(Main.number()-1).Points(people))+static_cast<int>(layout.card(Pair.at(i).second).Points(people)) > maxVal ){
                                maxVal = static_cast<int>(Main.card(Main.number()-1).Points(people))+static_cast<int>(layout.card(Pair.at(i).second).Points(people));
                                idx = Pair.at(i).second;
                            }
                        }                        
                        wcout << int_to_char(idx);
                        Sleep(TIME_CONSTANT);
                        X = idx;
                    }
                    else
                        X = GetKey();
                    wcout << endl;

                    //Capture and print the message.
                    if( CaptureCard( (int)(Main.number()-1), X, Main, layout) == 1 ){
                        system("cls");
                        wcout << L"Layout:\n";
                        layout.Print();
                        wcout << L"\nCards in hand:\n";
                        Hand.Print();
                        wcout << L"\nNumber of captured cards: " << Pile.number() << endl;
                    }
                    else{
                        wcout << L"Sorry, this action is invalid.\n";
                        continue;
                    }
                    break;
                }
                break;

            case 2:
                layout.Add(Main.Deal());

                wcout << L"\nNo cards are to be captured.\n";
                wcout << L"Put the card on the layout directly.\n";
                wcout << L"Press any key to continue...";

                if( ComputerOn()==true ) //Computer operation
                    Sleep(TIME_CONSTANT);
                else
                    _getch();

                //Print.
                system("cls");
                wcout << L"Layout:\n";
                layout.Print();
                wcout << L"\nCards in hand:\n";
                Hand.Print();
                break;

            default:
                continue;
        }
    }
}

inline void Player::finish_this_turn () {
    wcout << L"\nYour turn is going to finish.\n";
    wcout << L"\nPress any key to continue....\n";
    if( ComputerOn()==true ) //Computer operation
        Sleep(TIME_CONSTANT);
    else
        _getch();
}

void Player::Play (int people, Deck &Main, Deck &layout, Card card) { // each round's play
    //Print who's turn.
    print_whos_turn();

    //Print out layout and hand cards.
    print_layout_and_own_cards(layout);

    //First stage pairing
    first_stage_pairing(people, layout);

    //Second stage preparation
    second_stage_prepare(Main);

    //Special case check
    second_stage_special_case_check(Main, layout, card);

    //Second stage pairing
    second_stage_pairing(people, Main, layout);

    //Finish this turn.
    finish_this_turn();

    return;
}

void Player::SetName (wstring Name) { // Set the player's name.
    name = Name;
}

wstring Player::GetName () const { // Get the player's name.
    return name;
}

int Player::Points (int people) { // Count points for this player.
    int score = 0;
    for(int i=0; i<(int)GetPile().number(); i++)
        score += GetPile().card(i).Points(people);
    return score;
}

//Use operator overloading to simplify coding.
void Player::operator =(const Player &player) {
    Hand = player.GetHand();
    Pile = player.GetPile();
    name = player.name;
    computer = player.computer;
}
