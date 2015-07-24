//============================================================================
// name        : chinese_ten.cpp
//============================================================================

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include "card.h"
#include "deck.h"
#include "player.h"
#include "ranking.h"

using namespace std;

int main () {

    cout << "==========================Welcome to Chinese Ten!!!!========================\n\n";
    cout << "Loading";
    for(int i=5; i>0; i--){
        Sleep(200*i);
        cout << '.';
    }

    MENU:
    Deck Main, Layout;
    vector<Player> players;
    vector<Player> player_buffer;
    vector<int> order;
    string name;
    Card card;
    string str;
    stringstream strstream;
    Ranking ranking;
    Score score;
    int random, temp;
    int finish = 0;
    int out_of_range=0;
    int player_num=0;
    int computer_num=0;
    int player_index=-1;
    int tie_score=0;
    int check_people=0;
    char menu=0;

    //Initialize
    players.clear();
    players.resize(4);
    player_buffer.clear();
    player_buffer.resize(4);
    order.clear();
    order.resize(4);
    ranking.Clear();

    system("cls");
    cout << "==================================Main Menu=================================\n\n";  
    cout << "(1)Play the game!\n";
    cout << "(2)Check the ranking table.\n";
    cout << "( )Exit.\n";
    menu = getch();
    switch(menu){

        case '1': break;

        case '2':
            system("cls");
            cout << "===================Ranking Table Menu===================\n\n";
            cout << "(2)Two persons\n";
            cout << "(3)Three persons\n";
            cout << "(4)Four persons\n";
            cout << "( )Go back to main menu.\n";
            check_people = getch()-'0';
            if( !(2<=check_people && check_people<=4) )
                goto MENU;
            system("cls");
            cout << "===================Ranking Table Menu===================\n\n";
            cout << "(1)See top 10.\n";
            cout << "(2)Search for somebody's scores.\n";
            cout << "( )Go back to main menu.\n\n";
            menu = getch();
            switch(menu){
                case '1':
                    ranking.FindTop(check_people,10);
                    ranking.Print(10);
                    cout << "\nPress any key to go back to main menu...\n";
                    getch();
                    break;
                case '2':
                    cout << "I want to see <Name>'s scores. Name = ";
                    cin.sync();
                    getline(cin,name);
                    cout << endl;
                    if( ranking.FindByName(check_people,name)==0 )
                        cout << "There is no any " << name << "\'s record.\n";
                    else
                        ranking.Print();
                    cout << "\nPress any key to go back to main menu...\n";
                    getch();
                    break;
                default: break;
            }
            goto MENU;
            break;

        default: exit(EXIT_SUCCESS);
    }

    //Choose the number of players
    do{
        system("cls");
        cout << "Please choose the number of players ( 2-4 people ): ";
        player_num = getche() - '0';
        cout << endl;
        if( !(2<=player_num && player_num<=4) ){
            out_of_range = 1;
            cout << "Sorry, your input is out of range.\n";
            Sleep(1000);
        }
        else
            out_of_range = 0;
    }while( out_of_range==1 );

    //Choose the number of computer players
    cout << endl;
    do{
        for(int i=0; i<=player_num; i++)
            printf("(%d) %d human player(s), %d computer player(s)\n",i,player_num-i,i);
        cout << "\nMake a choice for the players' property. I want to choose ";
        computer_num = getche()-'0';
        cout << '.' << endl;
        if( !(0<=computer_num && computer_num<=player_num) ){
            out_of_range = 1;
            cout << "Sorry, your input is out of range.\n\n";
        }
        else
            out_of_range = 0;
    }while( out_of_range==1 );

    //Input the names.
    cout << "\nGive the players\' names!!\n\n";
    for(int i=0; i<player_num; i++){
        cout << "PLAYER" << ": ";
        cin.sync();
        name="";
        if( i<computer_num ){
            name = "COMPUTER_";
            name += (i+1+'0');
            cout << name << endl;
            players.at(i).EnableComputer();
        }
        else
            getline(cin,name);
        if( name == "" ){
            name = "PLAYER_";
            name += (i+'A');
        }
        players.at(i).SetName(name);
    }

    //Use clock to initialize the seed.
    srand(time(NULL));

    //Replicate a player to a buffer and set order = {0,1,2,3}.
    for(int i=0; i<player_num; i++){
        order.at(i) = i;
        player_buffer.at(i) = players.at(i);
    }

    //Rearrange(randomize) the array "order."
    for(int i=0; i<player_num; i++){
        random = rand() % player_num;
        temp = order.at(i);
        order.at(i) = order.at(random);
        order.at(random) = temp;
    }

    //Put the players in the buffer into the original player based on the randomized order.
    for(int i=0; i<player_num; i++)
        players.at(i) = player_buffer.at(order.at(i));

    //Print the rearraged player's order.
    system("cls");
    cout << "The playing order is:\n\n";
    for(int i=0; i<player_num; i++)
        cout << "Player" << i+1 << ": " << players.at(i).GetName() << endl;
    cout << "\n### Player1, " << players.at(0).GetName() << " is the so-called dealer. ###\n\n";

    //Waiting
    cout << "Press any key to continue...";
    cin.sync();
    getch();

    //Shuffle the cards.
    Main.Full();
    Main.Shuffle();

    // initial deal
    int init_card_num = 24 / player_num;
    for (int i=0; i<init_card_num; i++) {
        for (int j=0; j<player_num; j++) {
            players.at(j).GetCardToHand(Main.Deal());
        }
    }

    // flip 4 cards and put on table
    for(int i=0; i<4; i++)
        Layout.Add(Main.Deal());

    do{
        //Special-case check for four cards.
        while( Layout.special_check(4)!=static_cast<Card::Rank>(Unknown) ){
            card.set_rank( Layout.special_check(4) );
            system("cls");
            cout << "Now the layout contains four cards of rank \"" << card.rank_name() << "\"." << endl << endl;
            cout << "Layout:\n\n";
            Layout.Print();
            cout << "\nThis is the special case." << endl;
            cout << "The four cards are now being captured by the dealer, " << players.at(0).GetName() << ".\n\n";
            while( Layout.number()>0 )
                players.at(0).GetCardToPile(Layout.Deal());
            cout << "Capturing";
            for(int i=5; i>=0; i--){
                Sleep(500);
                cout <<'.';
            }
            cout << endl << endl;
            cout << "Now " << players.at(0).GetName() << " has captured the four cards:\n\n";
            players.at(0).GetPile().Print();
            for(int i=0; i<4; i++)
                Layout.Add(Main.Deal());
            cout << "\nThe new four cards flipped from the deck are now on the table.\n\nPress any key to continue...";
            cin.sync();

            //Computer Operation
            if( players.at(0).ComputerOn()==true )
                players.at(0).PressAnyKey();
            getch();
        };

        //Special-case check for three cards initially.
        while( Layout.special_check(3)!=static_cast<Card::Rank>(Unknown) ){
            card.set_rank( Layout.special_check(3) );
            for(int i=0; i<player_num && player_index==-1 ; i++)
                for(int j=0; j<players.at(i).GetHand().number() && player_index==-1 ; j++)
                    if( players.at(i).GetHand().card(j).rank_name() == card.rank_name() )
                        player_index = i;

            //If no one has the other card of the same rank now, get out of this while-loop.
            if( player_index==-1 )
                break;

            system("cls");
            cout << "Now the layout contains three cards of rank \"" << card.rank_name() << "\"." << endl << endl;
            cout << "Layout:\n\n";
            Layout.Print();
            cout << "\nThis is the special case." << endl;
            cout << "The three cards are now being captured by the player, " << players.at(player_index).GetName() << ", who has the other card of the same rank.\n\n";
            Layout.Sort();
            if( Layout.card(0).rank_name()==card.rank_name() ){
                for(int i=0; i<3; i++){
                    players.at(player_index).GetCardToPile(Layout.card(0));
                    Layout.Delete(0);
                }
            }
            else if( Layout.card(3).rank_name()==card.rank_name() ){
                for(int i=0; i<3; i++)
                    players.at(player_index).GetCardToPile(Layout.Deal());
            }
            else{

            }
            cout << "Capturing";
            for(int i=5; i>=0; i--){
                Sleep(500);
                cout <<'.';
            }
            cout << endl << endl;
            cout << "Now " << players.at(player_index).GetName() << " has captured the three cards:\n\n";
            players.at(player_index).GetPile().Print();
            for(int i=0; i<3; i++)
                Layout.Add(Main.Deal());
            cout << "\nThe new three cards flipped from the deck are now on the table.\n\nPress any key to continue...";
            cin.sync();
            //Computer Operation
            if( players.at(player_index).ComputerOn()==true )
                players.at(player_index).PressAnyKey();
            getch();
        };
    }while( Layout.special_check(4)!=static_cast<Card::Rank>(Unknown) );

    // play game!
    while ( finish == 0 ) {
        for (int i=0; i<player_num; i++)
            players.at(i).Play(player_num,Main,Layout,card);
        for (int i=0; i<player_num; i++){
            if( players.at(i).GetHand().number()==0 )
                finish = 1;
            else
                finish = 0;
        }
    }

    // count points for each player
    system("cls");
    cout << "========================This game has been finished!!!======================\n\n";
    for (int i=0; i<player_num; i++) {
        cout << players.at(i).GetName() << "\'s cards" << ":" << endl;
        players.at(i).GetPile().Print();
        cout << endl;
    }

    // scoreboard
    switch(player_num){
        case 2: tie_score=105; break;
        case 3: tie_score=80; break;
        case 4: tie_score=70; break;
        default: tie_score=0; break;
    }
    ranking.Clear();
    for(int i=0; i<player_num; i++){
        score.name = players.at(i).GetName();
        score.score = players.at(i).Points(player_num);
        ranking.Push_back(score);
    }
    //Sort the scores right after the game ends.
    ranking.SortByScore();

    cout << "================================Scoreboard==================================\n\n";
    cout << "Player: " << player_num << endl;
    cout << "Tie Score: " << tie_score << " points" << endl << endl;
    cout << "=Rank=|===============Name===============|====Points====|=======Score=======" << endl;
    for (int i=0; i<player_num; i++) {
        printf("  %02d  |",i+1);
        cout << setw(34) << ranking.GetPlayer(i).name << '|';
        cout << setw(5) << ranking.GetPlayer(i).score;
        cout << " point(s)|";
        if( ranking.GetPlayer(i).score>tie_score ){
            printf("  Win ");
            cout << setw(4) << ranking.GetPlayer(i).score-tie_score << " point(s)" << endl;
        }
        else if( ranking.GetPlayer(i).score<tie_score ){
            printf("  Lose");
            cout << setw(4) << tie_score-ranking.GetPlayer(i).score << " point(s)" << endl;
        }
        else if( ranking.GetPlayer(i).score==tie_score ){
            printf("        Tie\n");
        }
        else{
            printf("-------------------\n");
        }
    }
    cout <<  endl;

    //Record the game to a file.
    ranking.Recording(player_num);


    //Go back to main menu.
    cout << "Press any key to go back to main menu...\n";
    getch();
    goto MENU;

    return 0;
}
