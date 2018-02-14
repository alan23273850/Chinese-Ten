#include <iomanip>
#include <conio.h> // _getch
#include <io.h> // _setmode
#include <fcntl.h> // _fileno
#include "ranking.h"
#include "color.h"

using namespace std;

void prepare_console_settings () {
    _setmode(_fileno(stdout), _O_U8TEXT);
    ColorHandler::GetBackground();
}

void print_welcome_screen () {
    system("cls");
    wcout << L"==========================Welcome to Chinese Ten!!!!========================\n\n";
    wcout << L"Loading";
    for (int i=5; i>0; i--) {
        Sleep(200*i);
        wcout << '.';
    }
}

int print_main_menu () {
    system("cls");
    wcout << L"==================================Main Menu=================================\n\n";
    wcout << L"(1)Play the game!\n";
    wcout << L"(2)Check the ranking table.\n";
    wcout << L"( )Exit.\n";
    return _getch();
}

void check_the_ranking_table () {
    // Phase 1: Choose persons.
    system("cls");
    wcout << L"===================Ranking Table Menu===================\n\n";
    wcout << L"(2)Two persons\n";
    wcout << L"(3)Three persons\n";
    wcout << L"(4)Four persons\n";
    wcout << L"( )Go back to main menu.\n";
    int persons = _getch()-'0';
    if( !(2<=persons && persons<=4) )
        return;

    // Phase 2: Choose the query type.
    system("cls");
    wcout << L"===================Ranking Table Menu===================\n\n";
    wcout << L"(1)See top 10.\n";
    wcout << L"(2)Search for somebody's scores.\n";
    wcout << L"( )Go back to main menu.\n\n";
    int menu = _getch();

    // Phase 3: Implement the menu.
    Ranking ranking;
    wstring name;
    switch (menu) {
        case '1':
            ranking.FindTop(persons,10);
            ranking.Print(10);
            wcout << L"\nPress any key to go back to main menu...\n";
            _getch();
            break;
        case '2':
            wcout << L"I want to see <Name>'s scores. Name = ";
            wcin.sync();
            getline(wcin,name);
            wcout << endl;
            if( ranking.FindByName(persons,name)==0 )
                wcout << L"There is no any " << name << L"\'s record.\n";
            else
                ranking.Print();
            wcout << L"\nPress any key to go back to main menu...\n";
            _getch();
            break;
        default: break;
    }
}

void process_main_menu () {
    while (true) {
        switch (print_main_menu()) {
            case '1': return;
            case '2': check_the_ranking_table(); continue;
            default: exit(EXIT_SUCCESS);
        }
    }
}

vector<Player> choose_player_num () {
    while (true) {
        system("cls");
        wcout << L"Please choose the number of players ( 2-4 people ): ";
        int player_num = _getche() - '0';
        wcout << endl;
        if( !(2<=player_num && player_num<=4) ){
            wcout << L"Sorry, your input is out of range.\n";
            Sleep(1000);
            continue;
        }
        else
            return vector<Player>(player_num);
    }
}

int choose_computer_num (int player_num) {
    wcout << endl;
    while (true) {
        for(int i=0; i<=player_num; i++)
            wprintf(L"(%d) %d human player(s), %d computer player(s)\n", i, player_num-i, i);
        wcout << L"\nMake a choice for the players' property. I want to choose ";
        int computer_num = _getche() - '0';
        wcout << '.' << endl;
        if( !(0<=computer_num && computer_num<=player_num) ){
            wcout << L"Sorry, your input is out of range.\n\n";
            continue;
        }
        else
            return computer_num;
    }
}

void input_names (int computer_num, vector<Player> &players) {
    wstring name;
    wcout << L"\nGive the players\' names!!\n\n";
    for(int i=0; i<(int)players.size(); i++){
        wcout << L"PLAYER: ";
        wcin.sync();
        name = L"";
        if( i < computer_num ){
            name = L"COMPUTER_";
            name += (wchar_t)(i+1+'0');
            wcout << name << endl;
            players.at(i).EnableComputer();
        }
        else
            getline(wcin,name);
        if( name == L"" ){
            name = L"PLAYER_";
            name += (wchar_t)(i+'A');
        }
        players.at(i).SetName(name);
    }
}

void shuffle_player_order (vector<Player> &players) {
    // variables
    vector<Player> players_buffer;
    vector<int> order;
    srand((unsigned)time(NULL));
    // Duplicate a player to a buffer and set order = {0,1,2,3}.
    for(int i=0; i<(int)players.size(); i++){
        order.push_back(i);
        players_buffer.push_back(players.at(i));
    }
    // Rearrange(randomize) the array "order."

    for(int i=0; i<(int)players.size(); i++){
        int random = rand() % (int)players.size();
        int temp = order.at(i);
        order.at(i) = order.at(random);
        order.at(random) = temp;
    }
    // Put the players in the buffer into the original player based on the randomized order.
    for(int i=0; i<(int)players.size(); i++)
        players.at(i) = players_buffer.at(order.at(i));
}

void print_player_order (vector<Player> &players) {
    system("cls");
    wcout << L"The playing order is:\n\n";
    for(int i=0; i<(int)players.size(); i++)
        wcout << L"Player" << i+1 << L": " << players.at(i).GetName() << endl;
    wcout << L"\n### Player1, " << players.at(0).GetName() << L" is the so-called dealer. ###\n\n";
}

void print_card_results (vector<Player> &players) {
    system("cls");
    wcout << L"========================This game has been finished!!!======================\n\n";
    for (int i=0; i<(int)players.size(); i++) {
        wcout << players.at(i).GetName() << L"\'s cards" << L":" << endl;
        players.at(i).GetPile().Print();
        wcout << endl;
    }
}

Ranking print_scoreboard (vector<Player> &players) {
    Score score;
    Ranking ranking;
    int tie_score = 0;
    switch (players.size()) {
        case 2: tie_score = 105; break;
        case 3: tie_score = 80; break;
        case 4: tie_score = 70; break;
        default: tie_score = 0; break;
    }
    for(int i=0; i<(int)players.size(); i++){
        score.name = players.at(i).GetName();
        score.score = players.at(i).Points(players.size());
        ranking.Push_back(score);
    }
    //Sort the scores right after the game ends.
    ranking.SortByScore();

    wcout << L"================================Scoreboard==================================\n\n";
    wcout << L"Player: " << players.size() << endl;
    wcout << L"Tie Score: " << tie_score << L" points" << endl << endl;
    wcout << L"=Rank=|===============Name===============|====Points====|=======Score=======" << endl;
    for (int i=0; i<(int)players.size(); i++) {
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
    wcout << endl;
    return ranking;
}

void before_game (Deck &Main, Deck &Layout, vector<Player> &players) {
    // shuffle cards
    Main.Full();
    Main.Shuffle();

    // initial deal
    int init_card_num = 24 / players.size();
    for (int i=0; i<init_card_num; i++) {
        for (int j=0; j<(int)players.size(); j++) {
            players.at(j).GetCardToHand(Main.Deal());
        }
    }

    // flip 4 cards and put on table
    for(int i=0; i<4; i++)
        Layout.Add(Main.Deal());
}

void special_check (Deck &Main, Deck &Layout, vector<Player> &players, Card &card) {
    int player_index = -1;
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
                Sleep(800);
            else
                _getch();
        };

        //Special-case check for three cards initially.
        while( Layout.special_check(3)!=static_cast<Card::Rank>(Unknown) ){
            card.set_rank( Layout.special_check(3) );
            for(int i=0; i<(int)players.size() && player_index==-1 ; i++)
                for(int j=0; j<(int)players.at(i).GetHand().number() && player_index==-1 ; j++)
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
                Sleep(800);
            else
                _getch();
        };
    }while( Layout.special_check(4)!=static_cast<Card::Rank>(Unknown) );
}

void play_game (Deck &Main, Deck &Layout, vector<Player> &players, Card &card) {
    bool finish = false;
    while ( finish == false ) {
        for (int i=0; i<(int)players.size(); i++)
            players.at(i).Play((int)players.size(), Main, Layout, card);
        for (int i=0; i<(int)players.size(); i++){
            if( players.at(i).GetHand().number()==0 )
                finish = true;
            else
                finish = false;
        }
    }
}

int main () {
    // Variables
    Deck Main, Layout;
    vector<Player> players;
    Card card;
    Ranking ranking;
    int computer_num;

    // Initial settings
    prepare_console_settings();
    print_welcome_screen();

    while (true) {
        //Process the main menu (mainly the ranking table part).
        process_main_menu();

        //Choose the number of players
        players = choose_player_num();

        //Choose the number of computer players
        computer_num = choose_computer_num((int)players.size());

        //Input the names.
        input_names(computer_num, players);

        //Shuffle the order of players.
        shuffle_player_order(players);

        //Print the rearraged player's order.
        print_player_order(players);

        //Waiting
        wcout << L"Press any key to continue..."; wcin.sync(); _getch();

        //Do some preparation before the game starts.
        before_game(Main, Layout, players);

        //Perform special operations for some cases.
        special_check(Main, Layout, players, card);

        //Start to play the game!
        play_game(Main, Layout, players, card);

        //count points for each player
        print_card_results(players);

        //scoreboard
        ranking = print_scoreboard(players);

        //Record the game to a file.
        ranking.Recording((int)players.size());

        //Go back to main menu.
        wcout << L"Press any key to go back to main menu...\n"; _getch();
    }

    return 0;
}
