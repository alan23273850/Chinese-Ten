#include <iostream> // cout
#include <iomanip> // setw
#include "utility.h" // Getch, Getche, _sleep, Getline, clear_screen
#include "ranking.h" // Ranking
#include "color.h" // ColorHandler
#include "deck.h" // Deck
#include "player.h" // Player
#include "card.h" // Card

#ifdef _WIN32
    #include <windows.h> // SetConsoleOutputCP(CP_UTF8);
#endif

using namespace std; // cout

extern Deck source, layout;
extern vector<Player> players;
extern Ranking ranking_tables[3][2];

void prepare_console_settings()
{
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // make the console print UTF-8 characters
        ColorHandler::get_default(); // save the original color setting
    #else
        setbuf(stdout, NULL); // make the dot process visible
        init_termios(0); // make inputs at inappropriate times invisible
    #endif
}

void print_welcome_screen()
{
    clear_screen();
    cout << "============== Welcome to Chinese Ten!!!! ==============\n\n";
    cout << "Loading"; for (int i=5; i>0; i--) { _sleep(200*i); cout << '.'; }
}

void print_main_menu()
{
    clear_screen();
    cout << "======================= Main Menu ======================\n\n";
    cout << "(1) Play the game!\n";
    cout << "(2) Check the ranking table.\n";
    cout << "( ) Exit.\n";
}

void check_the_ranking_table()
{
    // Stage 1: Choose persons.
    clear_screen();
    cout << "================== Ranking Table Menu ==================\n\n";
    cout << "(2) Two persons\n";
    cout << "(3) Three persons\n";
    cout << "(4) Four persons\n";
    cout << "( ) Go back to main menu.\n";
    int persons = Getintfromch();
    if (!(2 <= persons&&persons <= 4))
        return;

    // Stage 2: Choose the query type.
    while (true) {
        clear_screen();
        cout << "================== Ranking Table Menu ==================\n\n";
        cout << "(1) See top 10.\n";
        cout << "(2) Search for somebody's scores.\n";
        cout << "( ) Go back to main menu.\n\n";
        int menu = Getch();

        // Stage 3: Implement the menu.
        if (menu == '1') {
            if (ranking_tables[persons-2][0].size() == 0) {
                ranking_tables[persons-2][0].read_file(persons);
                ranking_tables[persons-2][0].sort_by_score();
            }
            ranking_tables[persons-2][0].print(10);
        }
        else if (menu == '2') {
            if (ranking_tables[persons-2][1].size() == 0) {
                ranking_tables[persons-2][1].read_file(persons);
                ranking_tables[persons-2][1].sort_by_name();
            }
            cout << "I want to see <Name>'s scores. Name = ";
            string name; Getline(cin, name); cout << endl;
            Ranking ranking = ranking_tables[persons-2][1];
            if (!ranking.extract_by_name(name))
                cout << "There is no any " << name << "\'s record.\n";
            else
                ranking.print();
        } else
            break;
        cout << "\nPress any key to go back to the menu again...\n"; Getch();
    }
}

int choose_player_num()
{
    while (true) {
        clear_screen();
        cout << "Please choose the number of players (2-4 people): ";
        int player_num = Getintfromche(); cout << endl;
        if (!(2 <= player_num&&player_num <= 4)) {
            cout << "\nSorry, your input is out of range.";
            _sleep(1000);
        } else
            return player_num;
    }
}

int choose_computer_num(int player_num)
{
    for (int i=0; i<=player_num; i++)
        cout << '(' << i << ") " << player_num - i << " human player(s), " << i << " computer player(s)\n";
    while (true) {
        cout << "\nMake a choice for the players' property. I want to choose ";
        int com_num = Getintfromche(); cout << ".\n";
        if (0 <= com_num&&com_num <= player_num) return com_num;
        else cout << "Sorry, your input is out of range.\n";
    }
}

void input_names(int computer_num)
{
    cout << "\nGive the players\' names!!\n\n";
    for (int i=0; i<players.size(); i++) {
        string name;
        do {
            cout << "PLAYER " << i+1 << ": ";
            if (i < computer_num) {
                name = "COMPUTER_"; name += ('A'+i);
                cout << name << endl;
                players.at(i).enable_computer();
            } else
                Getline(cin, name);
        } while (name.find(',') != string::npos); // if comma in name
        if (name == "") {
            name = "PLAYER_"; name += ('A'+i);
            cout << "PLAYER " << i+1 << ": " << name << endl;
        }
        cout << endl;
        if (name.length() > 33) name.resize(33); // name should not be too long
        players.at(i).give_name(name);
    }
}

void print_player_order()
{
    clear_screen();
    cout << "The playing order is:\n\n";
    for (int i=0; i<players.size(); i++)
        cout << "Player " << i+1 << ": " << players.at(i).name() << endl;
    cout << "\n### Player1, " << players.at(0).name() << " is the so-called dealer. ###\n\n";
}

void shuffle_and_deal_cards()
{
    // shuffle cards
    source.fill_and_shuffle();

    // initial deal
    int init_card_num = 24 / players.size();
    for (int i=0; i<init_card_num; i++)
        for (int j=0; j<players.size(); j++)
            players.at(j).get_card_to_my_hand(source.deal());

    // flip 4 cards and put on the table
    layout = Deck(); // important when you "continue" to play the next game.
    for (int i=0; i<4; i++)
        layout.add(source.deal());
}

void play_game_loop(Card::Rank special_rank)
{
    for (int j=0; j<24/players.size(); j++)
        for (int i=0; i<players.size(); i++)
            players.at(i).play(special_rank);
}

void print_card_results()
{
    clear_screen();
    cout << "==================== This game has been finished!!! ====================\n\n";
    for (int i=0; i<players.size(); i++) {
        cout << players.at(i).name() << "\'s cards:\n";
        players.at(i).captured().print();
        cout << endl;
    }
}

Ranking print_scoreboard()
{
    // decide the tie score depending on the number of players
    int tie_score = 0;
    switch (players.size()) {
        case 2: tie_score = 105; break;
        case 3: tie_score = 80; break;
        case 4: tie_score = 70; break;
    }

    // push records of all the players
    Ranking ranking;
    for (int i=0; i<players.size(); i++)
        ranking.push_back(Score(players.at(i).name(),
                                players.at(i).points(players.size())));

    // sort the scores right after the game ends.
    ranking.sort_by_score();

    // print the scoreboard
    cout << "============================== Scoreboard ==============================\n\n";
    cout << "Player: " << players.size() << endl;
    cout << "Tie Score: " << tie_score << " points\n\n";
    cout << "=Rank=|============== Name ==============|=== Points ===|==== Score ====\n";
    for (int i=0; i<players.size(); i++) {
        cout << "  " << setfill('0') << setw(2) << i+1 << "  |" << setfill(' ');
        cout << setw(33) << ranking.record(i).name << " |";
        cout << setw(5) << ranking.record(i).score << " point(s)| ";
        if (ranking.record(i).score > tie_score)
            cout << '+' << setw(4) << ranking.record(i).score - tie_score << " point(s)\n";
        else if (ranking.record(i).score < tie_score)
            cout << '-' << setw(4) << tie_score - ranking.record(i).score << " point(s)\n";
        else // (ranking.record(i).score == tie_score)
            cout << "     Tie\n";
    }
    cout << endl;
    return ranking;
}

Card::Rank special_check()
{
    int who = -1;
    Card::Rank special_rank;

    // Special-case check for four cards.
    if ((special_rank = layout.special_check(4)) != Card::Rank::Unknown) // if the "initial" layout contains all four cards of the same rank
    {
        // distribute these cards to the so-called dealer.
        // otherwise no one can capture these cards and the game cannot be continued.
        who = 0;
        clear_screen();
        cout << "Now the layout contains all four cards of rank " << Card::rank_name(special_rank) << ".\n\n";
        cout << "Layout:\n\n"; layout.print();
        cout << "\nThis is the special case.\n";
        cout << "These cards are now being captured by the dealer, " << players.at(who).name() << ".\n\n";
        cout << "Capturing"; for (int i=5; i>0; i--) { _sleep(500); cout << '.'; }
        cout << "\n\nNow " << players.at(who).name() << " has captured all the cards:\n\n";
        while (layout.size() > 0)
            players.at(who).get_card_to_my_captured(layout.deal());
        players.at(who).captured().print();
    }

    // Special-case check for three cards initially.
    else if ((special_rank = layout.special_check(3)) != Card::Rank::Unknown) // if the layout contains three cards of the same rank
    {
        // find who has the last card of this rank
        for (int i=0; i<players.size(); i++)
            for (int j=0; j<players.at(i).hand().size(); j++)
                if (players.at(i).hand().card(j).rank() == special_rank)
                    who = i;

        // If no one has the last card of the same rank now,
        // return the special rank we have to be careful of in the future.
        if (who == -1) return special_rank;

        // otherwise simply distribute the three cards to that player.
        clear_screen();
        cout << "Now the layout contains three cards of rank " << Card::rank_name(special_rank) << ".\n\n";
        cout << "Layout:\n\n"; layout.print();
        cout << "\nThis is the special case.\n";
        cout << "The three cards are now being captured by the player, " << players.at(who).name() << ", who has the last card of the same rank.\n\n";
        cout << "Capturing"; for (int i=5; i>=0; i--) { _sleep(500); cout << '.'; }
        cout << "\n\nNow " << players.at(who).name() << " has captured the three cards:\n\n";
        for (int i=layout.size()-1; i>=0; i--) // traverse in reverse order so that indices of cards won't be changed.
            if (layout.card(i).rank() == special_rank)
                players.at(who).get_card_to_my_captured(layout.deal(i));
        players.at(who).captured().print();
    }
    else // if no special cases
        return Card::Rank::Unknown;

    // Computer Operation
    cout << "\nPress any key to continue...";
    if (players.at(who).is_computer())
        _sleep(COMPUTER_KEY_TIME);
    else
        Getch();
    return Card::Rank::Unknown;
}
