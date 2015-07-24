#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "player.h"
using namespace std;
const int TIME_CONSTANT=800;

int GetKey(){//Used to convert input char into the proper integer.
    char key;
        key = getche();
    if( '0'<=key && key<='9' )
        return key-'0';
    else if( 'a'<=key && key<='z' )
        return key-'a'+10;
    else if( 'A'<=key && key<='Z' )
        return key-'A'+10;
    else
        return -1;
}

char int_to_char(int n);

        Player::Player ( string Name ) { // constructor
            name = Name; //Note that the default name is "Player" defined in "player.h"
            Deck deck;
            Hand = deck;
            Pile = deck;
            computer = false;
        }

        void Player::GetCardToHand ( const Card &card ) { // Add a card to the player's Hand deck.
            Hand.Add(card);
        }

        void Player::GetCardToPile ( const Card &card ) { // Add a card to the player's Pile deck.
            Pile.Add(card);
        }

        Deck Player::GetHand () const {
            return Hand;
        }

        Deck Player::GetPile () const {
            return Pile;
        }

        int Player::CaptureCard ( int ToCapture_index, int layout_index, Deck &ToCapture , Deck &layout){
            //Use the card in "ToCapture" to capture the card in "layout."
            //Indexes help us to get the card we want.            

            if( !(0<=ToCapture_index && ToCapture_index<ToCapture.number() && 0<=layout_index && layout_index<layout.number()) )
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

        int Player::Play (int people, Deck &Main, Deck &layout, Card card) { // each round's play

            //Print who's turn.
            system("cls");
            cout << "=====================It\'s " << name << "\'s turn.=====================\n\n";

            int X, Y, success=0, count=0, max=0, min=0, index_x=0, index_y=0, index=0;
            char menu;
            Deck temp_deck;
            vector<int_pair> Pair;

            //Print out layout and hand cards.
            cout << "Layout:\n";
            layout.Print();
            cout << "\nCards in hand:\n";
            Hand.Print();

            for(unsigned int i=0; i<Find_match(Hand,layout).size(); i++)
                Pair.push_back( Find_match(Hand,layout).at(i) );

            //Show auto quick search.
            if( Print_match_first(Hand,layout,Pair)==1 )
                menu = '1';
            else
                menu = '2';

            //First stage
            MENU1:
            switch(menu){
                case '1':
                    //Computer Operation
                    if( ComputerOn()==true ){
                        max = static_cast<int>(Hand.card(Pair.at(0).x).Points(people))+static_cast<int>(layout.card(Pair.at(0).y).Points(people));
                        index_x = Pair.at(0).x;
                        index_y = Pair.at(0).y;
                        for(unsigned int i=0; i<Pair.size(); i++){
                            if( static_cast<int>(Hand.card(Pair.at(i).x).Points(people))+static_cast<int>(layout.card(Pair.at(i).y).Points(people)) > max ){
                                max = static_cast<int>(Hand.card(Pair.at(i).x).Points(people))+static_cast<int>(layout.card(Pair.at(i).y).Points(people));
                                index_x = Pair.at(i).x;
                                index_y = Pair.at(i).y;
                            }
                        }
                    }
                    cout << "\nI want to use my <X>th card to capture the <Y>th card.\n";

                    //Choose which cards to capture and to be captured.
                    cout << "X = ";
                    //Computer Operation
                    if( ComputerOn()==true ){                        
                        cout << int_to_char(index_x);
                        Sleep(TIME_CONSTANT);
                        X = index_x;
                    }
                    else
                        X = GetKey();
                    cout << endl;
                    if( X<0 || X>Hand.number()-1 ){
                        cout << "\nYour input index is out of range!!\n";
                        goto MENU1;
                    }
                    cout << "Y = ";
                    //Computer Operation
                    if( ComputerOn()==true ){                        
                        cout << int_to_char(index_y);
                        Sleep(TIME_CONSTANT);
                        Y = index_y;
                    }
                    else
                        Y = GetKey();
                    if( Y<0 || Y>layout.number()-1 ){
                        cout << "\nYour input index is out of range!!\n";
                        goto MENU1;
                    }

                    //Capture and print the message.
                    success = CaptureCard( X, Y, Hand, layout);
                    if( success == 1 ){
                        system("cls");
                        cout << "Layout:\n";
                        layout.Print();
                        cout << "\nCards in hand:\n";
                        Hand.Print();                        
                        cout << "\nNumber of captured cards: " << Pile.number() << endl;
                    }
                    else{
                        cout << "\nSorry, this action is invalid.\n";
                        goto MENU1;
                    }
                    break;

                case '2':
                    cout << "\nNo cards are to be captured.\n";
                    cout << "I want to discard my <X>th card. ";

                    //Choose which card to be discarded to the layout.
                    cout << "X = ";

                    //Computer operation
                    if( ComputerOn()==true ){
                        min = Hand.card(0).Points(people);
                        index = 0;
                        for(int i=0; i<Hand.number(); i++){
                            if( Hand.card(i).Points(people)<min ){
                                min = Hand.card(i).Points(people);
                                index = i;
                            }
                        }
                        X = index;
                        cout << int_to_char(X);
                        Sleep(TIME_CONSTANT);                        
                    }
                    else
                        X = GetKey();
                    cout << endl;
                    if( X<0 || X>Hand.number()-1 ){
                        cout << "\nYour input index is out of range!!\n";
                        goto MENU1;
                    }

                    //Discard.
                    layout.Add(Hand.card(X));
                    Hand.Delete(X);

                    //Print.
                    system("cls");
                    cout << "Layout:\n";
                    layout.Print();
                    cout << "\nCards in hand:\n";
                    Hand.Print();
                    break;

                default:
                    goto MENU1;
                    break;
            }

            //Second stage
            cout << "\nNow we are going to flip a card from the deck.\n";

            //Simulate the flipping process.
            cout << "Waiting";
            for(int i=6; i>=0; i--){
                Sleep(130*i);
                cout << '.';
            }

            //Print the flipped card.
            cout << "\nThe flipped card is: ";
            Main.card(Main.number()-1).Print();
            cout << endl;            

            //Special-case check for three cards.
            if( Main.card(Main.number()-1).rank_name()==card.rank_name() ){

                cout << "\nNow the layout contains three cards of rank \"" << card.rank_name() << ".\"" << endl << endl;
                cout << "This is the special case." << endl;
                cout << "You are going to capture the three cards on the layout with your ";
                if( Main.card(Main.number()-1).suit_name() == "Diamond" || Main.card(Main.number()-1).suit_name() == "Heart" )
                    SetColor(4,7);
                else
                    SetColor(0,7);
                cout << Main.card(Main.number()-1).suit_icon() << Main.card(Main.number()-1).rank_name();
                SetColor();
                cout << ".\n\n";

                layout.Sort();

                //Capture
                for(int i=0; i<layout.number() && count<3; i++){
                    if( layout.card(i).rank_name()==card.rank_name() ){
                        GetCardToPile(layout.card(i));
                        layout.Delete(i);
                        count++;
                        i--;
                    }
                }
                GetCardToPile(Main.Deal());

                cout << "Capturing";
                for(int i=5; i>=0; i--){
                    Sleep(500);
                    cout <<'.';
                }
                cout << endl << endl;
                cout << "Now you have captured all cards of rank \"" << card.rank_name() << ".\"\n";
                cout << "\nNumber of your all captured cards: " << Pile.number() << endl;
                cout << "\nYour turn is going to finish.\n";
                cout << "\nPress any key to continue...";
                cin.sync();
                //Computer operation
                if( ComputerOn()==true )
                    PressAnyKey();
                getch();
                return 0;
            }

            //Show auto quick search.
            temp_deck.Add(Main.card(Main.number()-1));
            if( Print_match_second(layout,Find_match(temp_deck,layout))==1 )
                menu = '1';
            else
                menu = '2';

            MENU2:
            switch(menu){
                case '1':                
                    cout << "\nI want to capture the <X>th card. ";

                    //Choose which cards to capture and to be captured.
                    cout << "X = ";

                    //Computer operation
                    temp_deck.Clear();
                    Pair.clear();
                    temp_deck.Add(Main.card(Main.number()-1));
                    for(unsigned int i=0; i<Find_match(temp_deck,layout).size(); i++)
                        Pair.push_back( Find_match(temp_deck,layout).at(i) );
                    if( ComputerOn()==true ){
                        int max = static_cast<int>(Main.card(Main.number()-1).Points(people))+static_cast<int>(layout.card(Pair.at(0).y).Points(people));
                        int index = Pair.at(0).y;
                        for(unsigned int i=0; i<Pair.size(); i++){
                            if( static_cast<int>(Main.card(Main.number()-1).Points(people))+static_cast<int>(layout.card(Pair.at(i).y).Points(people)) > max ){
                                max = static_cast<int>(Main.card(Main.number()-1).Points(people))+static_cast<int>(layout.card(Pair.at(i).y).Points(people));
                                index = Pair.at(i).y;
                            }
                        }                        
                        cout << int_to_char(index);
                        Sleep(TIME_CONSTANT);
                        X = index;
                    }
                    else
                        X = GetKey();
                    cout << endl;

                    //Capture and print the message.
                    success = CaptureCard( Main.number()-1, X, Main, layout);
                    if( success == 1 ){
                        system("cls");
                        cout << "Layout:\n";
                        layout.Print();
                        cout << "\nCards in hand:\n";
                        Hand.Print();
                        cout << "\nNumber of captured cards: " << Pile.number() << endl;                        
                    }
                    else{
                        cout << "Sorry, this action is invalid.\n";
                        goto MENU2;
                    }
                    break;

                case '2':
                    cout << "\nNo cards are to be captured.\n";
                    cout << "Put the card on the layout directly.\n";
                    layout.Add(Main.Deal());
                    cout << "Press any key to continue...";
                    //Computer operation
                    if( ComputerOn()==true )
                        PressAnyKey();
                    getch();

                    //Print.
                    system("cls");
                    cout << "Layout:\n";
                    layout.Print();
                    cout << "\nCards in hand:\n";
                    Hand.Print();
                    break;

                default:
                    goto MENU2;
                    break;
            }
            cout << "\nYour turn is going to finish.\n";
            cout << "\nPress any key to continue....\n";
            //Computer operation
            if( ComputerOn()==true )
                PressAnyKey();
            getch();
            return 0;
        }

        void Player::SetName( string Name ){ // Set the player's name.
            name = Name;
        }

        string Player::GetName( ) const{ // Get the player's name.
            return name;
        }

        int Player::Points ( int people ) { // Count points for this player.
            int score = 0;
            for(int i=0; i<GetPile().number(); i++)
                score += GetPile().card(i).Points(people);
            return score;
        }

        //Use operator overloading to simplify coding.
        void Player::operator =(const Player &player){
            Hand = player.GetHand();
            Pile = player.GetPile();
            name = player.name;
            computer = player.computer;
        }
