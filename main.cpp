//============================================================================
// name        : chinese_ten.cpp
//============================================================================

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sstream>
#include "card.h"
#include "deck.h"
#include "player.h"
#include "ranking.h"
#include "color.h"

using namespace std;

int main () {

    // pre-setting
    _setmode(_fileno(stdout), _O_U8TEXT);
    ColorHandler::GetBackground();

    wcout << L"==========================Welcome to Chinese Ten!!!!========================\n\n";
    wcout << L"Loading";
    for(int i=5; i>0; i--){
        Sleep(200*i);
        wcout << '.';
    }

    MENU:
    Deck Main, Layout;
    vector<Player> players;
    vector<Player> player_buffer;
    vector<int> order;
    wstring name;
    Card card;
    wstring str;
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
    wcout << L"==================================Main Menu=================================\n\n";
    wcout << L"(1)Play the game!\n";
    wcout << L"(2)Check the ranking table.\n";
    wcout << L"( )Exit.\n";
    menu = getch();
    switch(menu){

        case '1': break;

        case '2':
            system("cls");
            wcout << L"===================Ranking Table Menu===================\n\n";
            wcout << L"(2)Two persons\n";
            wcout << L"(3)Three persons\n";
            wcout << L"(4)Four persons\n";
            wcout << L"( )Go back to main menu.\n";
            check_people = getch()-'0';
            if( !(2<=check_people && check_people<=4) )
                goto MENU;
            system("cls");
            wcout << L"===================Ranking Table Menu===================\n\n";
            wcout << L"(1)See top 10.\n";
            wcout << L"(2)Search for somebody's scores.\n";
            wcout << L"( )Go back to main menu.\n\n";
            menu = getch();
            switch(menu){
                case '1':
                    ranking.FindTop(check_people,10);
                    ranking.Print(10);
                    wcout << L"\nPress any key to go back to main menu...\n";
                    getch();
                    break;
                case '2':
                    wcout << L"I want to see <Name>'s scores. Name = ";
                    wcin.sync();
                    getline(wcin,name);
                    wcout << endl;
                    if( ranking.FindByName(check_people,name)==0 )
                        wcout << L"There is no any " << name << L"\'s record.\n";
                    else
                        ranking.Print();
                    wcout << L"\nPress any key to go back to main menu...\n";
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
        wcout << L"Please choose the number of players ( 2-4 people ): ";
        player_num = getche() - '0';
        wcout << endl;
        if( !(2<=player_num && player_num<=4) ){
            out_of_range = 1;
            wcout << L"Sorry, your input is out of range.\n";
            Sleep(1000);
        }
        else
            out_of_range = 0;
    }while( out_of_range==1 );

    //Choose the number of computer players
    wcout << endl;
    do{
        for(int i=0; i<=player_num; i++)
            wprintf(L"(%d) %d human player(s), %d computer player(s)\n",i,player_num-i,i);
        wcout << L"\nMake a choice for the players' property. I want to choose ";
        computer_num = getche()-'0';
        wcout << '.' << endl;
        if( !(0<=computer_num && computer_num<=player_num) ){
            out_of_range = 1;
            wcout << L"Sorry, your input is out of range.\n\n";
        }
        else
            out_of_range = 0;
    }while( out_of_range==1 );

    //Input the names.
    wcout << L"\nGive the players\' names!!\n\n";
    for(int i=0; i<player_num; i++){
        wcout << L"PLAYER" << L": ";
        wcin.sync();
        name=L"";
        if( i<computer_num ){
            name = L"COMPUTER_";
            name += (i+1+'0');
            wcout << name << endl;
            players.at(i).EnableComputer();
        }
        else
            getline(wcin,name);
        if( name == L"" ){
            name = L"PLAYER_";
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
    wcout << L"The playing order is:\n\n";
    for(int i=0; i<player_num; i++)
        wcout << L"Player" << i+1 << L": " << players.at(i).GetName() << endl;
    wcout << L"\n### Player1, " << players.at(0).GetName() << L" is the so-called dealer. ###\n\n";

    //Waiting
    wcout << L"Press any key to continue...";
    wcin.sync();
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
            wcout << L"Now the layout contains four cards of rank \"" << card.rank_name() << L"\"." << endl << endl;
            wcout << L"Layout:\n\n";
            Layout.Print();
            wcout << L"\nThis is the special case." << endl;
            wcout << L"The four cards are now being captured by the dealer, " << players.at(0).GetName() << L".\n\n";
            while( Layout.number()>0 )
                players.at(0).GetCardToPile(Layout.Deal());
            wcout << L"Capturing";
            for(int i=5; i>=0; i--){
                Sleep(500);
                wcout <<'.';
            }
            wcout << endl << endl;
            wcout << L"Now " << players.at(0).GetName() << L" has captured the four cards:\n\n";
            players.at(0).GetPile().Print();
            for(int i=0; i<4; i++)
                Layout.Add(Main.Deal());
            wcout << L"\nThe new four cards flipped from the deck are now on the table.\n\nPress any key to continue...";
            wcin.sync();

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
            wcout << L"Now the layout contains three cards of rank \"" << card.rank_name() << L"\"." << endl << endl;
            wcout << L"Layout:\n\n";
            Layout.Print();
            wcout << L"\nThis is the special case." << endl;
            wcout << L"The three cards are now being captured by the player, " << players.at(player_index).GetName() << L", who has the other card of the same rank.\n\n";
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
            wcout << L"Capturing";
            for(int i=5; i>=0; i--){
                Sleep(500);
                wcout <<'.';
            }
            wcout << endl << endl;
            wcout << L"Now " << players.at(player_index).GetName() << L" has captured the three cards:\n\n";
            players.at(player_index).GetPile().Print();
            for(int i=0; i<3; i++)
                Layout.Add(Main.Deal());
            wcout << L"\nThe new three cards flipped from the deck are now on the table.\n\nPress any key to continue...";
            wcin.sync();
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
    wcout << L"========================This game has been finished!!!======================\n\n";
    for (int i=0; i<player_num; i++) {
        wcout << players.at(i).GetName() << L"\'s cards" << L":" << endl;
        players.at(i).GetPile().Print();
        wcout << endl;
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

    wcout << L"================================Scoreboard==================================\n\n";
    wcout << L"Player: " << player_num << endl;
    wcout << L"Tie Score: " << tie_score << L" points" << endl << endl;
    wcout << L"=Rank=|===============Name===============|====Points====|=======Score=======" << endl;
    for (int i=0; i<player_num; i++) {
        wprintf(L"  %02d  |",i+1);
        wcout << setw(34) << ranking.GetPlayer(i).name << '|';
        wcout << setw(5) << ranking.GetPlayer(i).score;
        wcout << L" point(s)|";
        if( ranking.GetPlayer(i).score>tie_score ){
            wprintf(L"  Win ");
            wcout << setw(4) << ranking.GetPlayer(i).score-tie_score << L" point(s)" << endl;
        }
        else if( ranking.GetPlayer(i).score<tie_score ){
            wprintf(L"  Lose");
            wcout << setw(4) << tie_score-ranking.GetPlayer(i).score << L" point(s)" << endl;
        }
        else if( ranking.GetPlayer(i).score==tie_score ){
            wprintf(L"        Tie\n");
        }
        else{
            wprintf(L"-------------------\n");
        }
    }
    wcout <<  endl;

    //Record the game to a file.
    ranking.Recording(player_num);


    //Go back to main menu.
    wcout << L"Press any key to go back to main menu...\n";
    getch();
    goto MENU;

    return 0;
}
