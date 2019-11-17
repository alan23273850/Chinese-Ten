#include <iostream> // cout
#include "utility.h" // Getch, shuffle
#include "body.h" // subroutines
#include "deck.h" // Deck
#include "player.h" // Player
#include "card.h" // Card
#include "ranking.h" // Ranking

using namespace std; // cout

// Variables
Deck source, layout;
vector<Player> players;
Ranking ranking_tables[3][2];

int main()
{
    // Initial settings
    prepare_console_settings();
    print_welcome_screen();

    while (true) {
        while (true) { // Process the main menu (mainly the ranking table part).
            print_main_menu();
            int ch = Getch();
            if (ch == '1') { /* (1) Play the game! */
                for (int i=0; i<3; i++)
                    for (int j=0; j<2; j++)
                        ranking_tables[i][j].clear();
                break;
            }
            else if (ch == '2') /* (2) Check the ranking table. */
                check_the_ranking_table();
            else {
                #ifdef __linux__
                    reset_original_termios(); // restore original setting after the game terminates.
                #endif
                exit(EXIT_SUCCESS);
            }
        }

        // Create a number of players.
        int player_num = choose_player_num();
        players = vector<Player>(player_num);

        // Choose the number of computer players.
        int computer_num = choose_computer_num(player_num);

        // Input the names.
        input_names(computer_num);

        // shuffle the order of players.
        shuffle(players);

        // print the rearraged player's order.
        print_player_order();

        // Simulate waiting.
        cout << "Press any key to continue..."; Getch();

        // Do some preparation before the game starts.
        shuffle_and_deal_cards();

        // Perform special operations for some cases.
        Card::Rank special_rank = special_check();

        // Start to play the game!
        play_game_loop(special_rank);

        // count points for each player
        print_card_results();

        // scoreboard
        Ranking ranking = print_scoreboard();

        // Record the game to a file.
        ranking.recording();

        // Go back to main menu.
        cout << "Press any key to go back to the main menu...\n"; Getch();
    }

    return 0;
}
