#include <iostream> // wout
#include "utility.h" // Getch, clear_screen
#include "player.h" // Player
#include "color.h" // ColorHandler

using namespace std; // wout

extern Deck source, layout;
extern vector<Player> players;

Player::Player(string name) { // constructor
    my_name = name; // Note that the default my_name is "Player" defined in "player.h"
    my_hand = Deck();
    my_captured = Deck();
    computer = false;
}

int Player::points(int people) { // Calculate points for this player.
    int score = 0;
    for (int i=0; i<my_captured.size(); i++)
        score += my_captured.card(i).points(people);
    return score;
}

// Use operator overloading to simplify coding.
void Player::operator=(const Player &player) {
    my_hand = player.my_hand;
    my_captured = player.my_captured;
    my_name = player.my_name;
    computer = player.computer;
}

void Player::play(Card::Rank special_rank) // each round's play
{
    // print who's turn
    clear_screen();
    cout << ">>> It\'s " << my_name << "\'s turn...\n\n";

    // print the layout and the player's own cards
    cout << "Layout:\n"; layout.print();
    cout << "\nCards in hand:\n"; my_hand.print();

    // Find which card in my hand can capture a card in the layout.
    first_stage_pairing();

    // second stage preparation
    cout << "\nNow we are going to flip a card from the deck. (" << source.size() << " cards remaining)\n";
    cout << "Waiting"; for (int i=6; i>0; i--) { _sleep(130*i); cout << '.'; }
    cout << "\nThe flipped card is: "; source.top().print();
    cout << endl;

    // Find which card in the layout can be captured by the flipped card.
    // If cards in the layout satisfy the special case, simply capture them all.
    if (!second_stage_special_case_check(special_rank))
        second_stage_pairing();

    // finish this turn
    cout << "\nYour turn is going to finish.\n";
    cout << "\nPress any key to continue....\n";
    if (is_computer()) // Computer operation
        _sleep(COMPUTER_KEY_TIME);
    else
        Getch();
}

void Player::first_stage_pairing()
{
    // Variables
    int x, y, people = players.size();

    // Find which card in my hand can capture a card in the layout.
    vector<pair<pair<int, Card>, pair<int, Card>>> pairs = find_match(my_hand, layout);

    // Show auto quick search.
    if (print_match_first(pairs)) // if a match exists, capture it.
        while (true) {
            // Print prompt message
            cout << "\nI want to use my <X>th card to capture the <Y>th card.\n";

            // Computer Operation: Choose its own answer by heuristic first.
            if (is_computer()) {
                int max = -1;
                for (int i=0; i<pairs.size(); i++) {
                    int sum = pairs.at(i).first.second.points(people) + pairs.at(i).second.second.points(people);
                    if (sum > max) {
                        max = sum;
                        x = pairs.at(i).first.first;
                        y = pairs.at(i).second.first;
                    }
                }
            }

            // Choose which cards (x) to capture and to be captured.
            while (true) {
                cout << "X = ";
                if (is_computer()) { // Computer Operation
                    cout << int_to_char(x);
                    _sleep(COMPUTER_KEY_TIME);
                } else
                    x = Getintfromche();
                cout << endl;
                if (0 <= x&&x <= my_hand.size()-1) break;
                else // Check out of range
                    cout << "\nYour input index is out of range!!\n";
            }

            // Choose which cards (y) to capture and to be captured.
            while (true) {
                cout << "Y = ";
                if (is_computer()) { // Computer Operation
                    cout << int_to_char(y);
                    _sleep(COMPUTER_KEY_TIME);
                } else
                    y = Getintfromche();
                if (0 <= y&&y <= layout.size()-1) break;
                else // Check out of range
                    cout << "\nYour input index is out of range!!\n";
            }

            // Perform "capture" action after the cards are chosen.
            if (Card::can_capture(my_hand.card(x), layout.card(y))) {
                my_captured.add(my_hand.deal(x));
                my_captured.add(layout.deal(y));
                break;
            } else
                cout << "\nSorry, this action is invalid.\n";
        }
    else { // if no match exists, simply discard one card.
        // Print prompt message.
        cout << "\nNo cards can be captured.\n\n";
        cout << "I want to discard my <X>th card. ";

        while (true) { // Choose which card to be discarded to the layout.
            cout << "X = ";
            if (is_computer()) { // Computer operation
                int min = 2147483647;
                for (int i=0; i<my_hand.size(); i++)
                    if (my_hand.card(i).points(people) < min) {
                        min = my_hand.card(i).points(people);
                        x = i;
                    }
                cout << int_to_char(x);
                _sleep(COMPUTER_KEY_TIME);
            } else
                x = Getintfromche();
            cout << endl;
            if (0 <= x&&x <= my_hand.size()-1) break;
            else cout << "\nYour input index is out of range!!\n";
        }

        // Perform "discard" action after the card is chosen.
        layout.add(my_hand.deal(x));
    }

    // Print the current status.
    clear_screen();
    cout << "Layout:\n"; layout.print();
    cout << "\nCards in hand:\n"; my_hand.print();
    cout << "\nTotal number of captured cards: " << my_captured.size() << endl;
}

bool Player::second_stage_special_case_check(Card::Rank special_rank)
{
    if (source.top().rank() == special_rank) {
        cout << "\nNow the layout contains three cards of rank " << Card::rank_name(special_rank) << ".\n\n";
        cout << "This is the special case.\n";
        cout << "You are going to capture the three cards on the layout with your "; source.top().print();
        cout << ".\n\n";

        // Capture the three cards.
        for (int i=layout.size()-1; i>=0; i--) // traverse in reverse order so that cards' indices will not be modified.
            if (layout.card(i).rank() == special_rank)
                my_captured.add(layout.deal(i));
        my_captured.add(source.deal());

        cout << "Capturing"; for (int i=5; i>=0; i--) { _sleep(500); cout << '.'; }
        cout << "\n\nNow you have captured all cards of rank " << Card::rank_name(special_rank) << ".\n";
        cout << "\nNumber of all your captured cards: " << my_captured.size() << endl;
        return true;
    }
    return false;
}

void Player::second_stage_pairing()
{
    // Find which card in the layout can be captured by the top card on the main deck.
    vector<pair<pair<int, Card>, pair<int, Card>>> pairs = find_match(source.top(), layout);

    // Show auto quick search.
    if (print_match_second(pairs)) // if a match exists, capture it.
        while (true) {
            int x, people = players.size(); // Variables

            // Choose which cards to capture and to be captured.
            cout << "\nI want to capture the <X>th card. ";
            cout << "X = ";

            // Computer operation
            if (is_computer()) {
                int max = -1;
                for (int i=0; i<pairs.size(); i++) {
                    int sum = pairs.at(i).first.second.points(people) + pairs.at(i).second.second.points(people);
                    if (sum > max) {
                        max = sum;
                        x = pairs.at(i).second.first;
                    }
                }
                cout << int_to_char(x);
                _sleep(COMPUTER_KEY_TIME);
            } else
                x = Getintfromche();
            cout << endl;

            // Perform capture action.
            if (Card::can_capture(source.top(), layout.card(x))) {
                my_captured.add(source.deal());
                my_captured.add(layout.deal(x));
                break;
            } else
                cout << "Sorry, this action is invalid.\n";
        }
    else { // if no match exists, simply pick the top card onto the layout.
        // Print prompt message.
        cout << "\nNo cards can be captured.\n";
        cout << "Put the card on the layout directly.\n";
        layout.add(source.deal()); // do action

        cout << "Press any key to continue...";
        if (is_computer()) // Computer operation
            _sleep(COMPUTER_KEY_TIME);
        else
            Getch();
    }

    // Print the current status.
    clear_screen();
    cout << "Layout:\n"; layout.print();
    cout << "\nCards in hand:\n"; my_hand.print();
    cout << "\nNumber of captured cards: " << my_captured.size() << endl;
}
