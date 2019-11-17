#include <iostream> // cout
#include <iomanip> // setw
#include "utility.h" // int_to_char, sort functions, shuffle
#include "deck.h" // Deck
#include "color.h" // ColorHandler

using namespace std; // cout

void Deck::fill_and_shuffle() { // Create a shuffled full deck.
    cards.clear();
    for (int j=1; j<=13; j++)
        for (int i=1; i<=4; i++)
            cards.push_back(Card((Card::Suit)i, (Card::Rank)j));
    shuffle(cards);

    // Note that the function of each index is mapped to the following.
    // [0, 1, 2, ..., 21, 22, 23][24, 25, 26, 27][28, 29, ...., 49, 50, 51]
    // <--- later distribute ---><initial layout><--- first distribute --->
    // Also each rank "n" is mapped to the index range [4*(n-1)..4n-1].

    // Generate a special case where rank "13" is moved to the initial layout.
    // for (int i=0; i<4; i++) {
    //     Card card = cards.at(4*(13-1)+i);
    //     cards.at(4*(13-1)+i) = cards.at(24+i);
    //     cards.at(24+i) = card;
    // }

    // Generate a special case where
    // one card of the initial layout is moved to the "first distribute" section.
    // Card card = cards.at(51);
    // cards.at(51) = cards.at(24);
    // cards.at(24) = card;

    // Generate a special case where
    // one card of the initial layout is moved to the "later distribute" section.
    // Card card = cards.at(23);
    // cards.at(23) = cards.at(24);
    // cards.at(24) = card;
}

void Deck::sort() // Sort the deck with bubble sort. Rank in ascending order. Suit in descending order.
{
    // rank first
    bubble_sort(cards, Card::cmp_by_rank);

    // then suit
    int end=0, start;
    while (end+1 < cards.size()) { // make "end" run in [0..N-2]
        start = end;
        while (end+1 < cards.size() // Use index boundary check to avoid error.
            && cards.at(start).rank() == cards.at(end+1).rank())
            end++;
        // So far, we ensure the elements in [start..end] have the same rank. Then
        // we want to perform bubble sort in this interval. Even if start==end, the
        // result is correct because no sorting is executed.
        bubble_sort(cards, start, end, Card::cmp_by_suit);

        // So far, we guarantee [end] and [end+1] must have different ranks, so we can
        // definitely increment our base pointer and disregard all things before [end+1].
        end++;
    }
}

void Deck::print() { // print all the cards in the deck
    sort();

    // print the order.
    for (int i=0; i<cards.size(); i++)
        cout << " (" << int_to_char(i) << ')';
    cout << endl;

    // print the suit.
    for (int i=0; i<cards.size(); i++) {
        cout << ' ';
        ColorHandler::set_color(cards.at(i).suit()==Card::Suit::Diamond || cards.at(i).suit()==Card::Suit::Heart);
        cout << ' ' << cards.at(i).suit_icon() << ' ';
        ColorHandler::set_default();
    }
    // print a full-width whitespace to avoid the card color extension
    // when the width of a window changes.
    cout << string({(char)0xE3, (char)0x80, (char)0x80}) << endl;

    // print the rank.
    for (int i=0; i<cards.size(); i++) {
        cout << ' ';
        ColorHandler::set_color(cards.at(i).suit()==Card::Suit::Diamond || cards.at(i).suit()==Card::Suit::Heart);
        cout << setw(2) << cards.at(i).rank_name() << ' ';
        ColorHandler::set_default();
    }
    // print a full-width whitespace to avoid the card color extension
    // when the width of a window changes.
    cout << string({(char)0xE3, (char)0x80, (char)0x80}) << endl;
}

Card::Rank Deck::special_check(int n) { // n = 3 (or 4), return three (or four) cards of "which" rank
    int five=0, ten=0, jack=0, queen=0, king=0;
    for (int i=0; i<cards.size(); i++)
        switch (cards.at(i).rank()) {
            case Card::Rank::Five: five++; break;
            case Card::Rank::Ten: ten++; break;
            case Card::Rank::Jack: jack++; break;
            case Card::Rank::Queen: queen++; break;
            case Card::Rank::King: king++; break;
        }
    if (five == n)
        return Card::Rank::Five;
    else if (ten == n)
        return Card::Rank::Ten;
    else if (jack == n)
        return Card::Rank::Jack;
    else if (queen == n)
        return Card::Rank::Queen;
    else if (king == n)
        return Card::Rank::King;
    else
        return Card::Rank::Unknown;
}

vector<pair<pair<int, Card>, pair<int, Card>>> find_match(const Deck &deck1, const Deck &deck2) {
    vector<pair<pair<int, Card>, pair<int, Card>>> result;
    for (int i=0; i<deck1.size(); i++)
        for (int j=0; j<deck2.size(); j++) {
            Card card1 = deck1.card(i);
            Card card2 = deck2.card(j);
            if (Card::can_capture(card1, card2))
                result.push_back(pair<pair<int, Card>, pair<int, Card>>
                    (pair<int, Card>(i, card1), pair<int, Card>(j, card2)));
        }
    return result;
}

vector<pair<pair<int, Card>, pair<int, Card>>> find_match(const Card &card, const Deck &deck2) {
    Deck deck1;
    deck1.add(card);
    return find_match(deck1, deck2);
}

bool print_match_first(const vector<pair<pair<int, Card>, pair<int, Card>>> &pairs) {
    if (pairs.size() == 0) return false;
    cout << "\nHere is a quick search for you.\n\n";
    for (int i=0; i<pairs.size(); i++) {
        cout << "You can use your card (" << int_to_char(pairs.at(i).first.first) << ')';
        pairs.at(i).first.second.print();
        cout << " to capture card (" << int_to_char(pairs.at(i).second.first) << ')';
        pairs.at(i).second.second.print();
        cout << ".\n";
    }
    return true;
}

bool print_match_second(const vector<pair<pair<int, Card>, pair<int, Card>>> &pairs) {
    if (pairs.size() == 0) return false;
    cout << "\nHere is a quick search for you.\n\n";
    cout << "You can capture card:";
    for (int i=0; i<pairs.size(); i++) {
        cout << " (" << int_to_char(pairs.at(i).second.first) << ')';
        pairs.at(i).second.second.print();
    }
    cout << ".\n";
    return true;
}
