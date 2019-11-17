#include <iostream> // string
#include "card.h" // Card
#include "color.h" // ColorHandler

using namespace std; // string

// Return the suit's icon in unicode format.
string Card::suit_icon() const {
    switch (my_suit) {
        case Suit::Spade: return string({(char)0xE2, (char)0x99, (char)0xA0}); // U+2660
        case Suit::Heart: return string({(char)0xE2, (char)0x99, (char)0xA5}); // U+2665
        case Suit::Diamond: return string({(char)0xE2, (char)0x99, (char)0xA6}); // U+2666
        case Suit::Club: return string({(char)0xE2, (char)0x99, (char)0xA3}); // U+2663
        default: return "";
    }
}

int Card::points(int people) {
    if (my_suit==Suit::Spade && my_rank==Rank::Ace // Ace of spades
        && (people==3 || people==4)) // in the 3 or 4 people case
        return 30;
    if (my_suit==Suit::Club && my_rank==Rank::Ace // Ace of clubs
        && people==4) // in the 4 people case
        return 40;
    if (my_suit==Suit::Heart || my_suit==Suit::Diamond) { // red
        if (my_rank == Rank::Ace) // Ace
            return 20;
        if (Rank::Nine <= my_rank&&my_rank <= Rank::King) // 9-King
            return 10;
        if (Rank::Two <= my_rank&&my_rank <= Rank::Eight) // 2-8
            return (int)my_rank;
    }
    return 0;
}

void Card::print() const {
    ColorHandler::set_color(my_suit==Suit::Diamond || my_suit==Suit::Heart);
    cout << suit_icon() << rank_name();
    ColorHandler::set_default();
}

bool Card::can_capture(Card card1, Card card2) {
    int rank_1 = (int)card1.rank();
    int rank_2 = (int)card2.rank();
    return 1<=rank_1&&rank_1<=9 && rank_1+rank_2==10
        || 10<=rank_1&&rank_1<=13 && rank_1==rank_2;
}

const string Card::suit_names[] = {"None", "Club", "Diamond", "Heart", "Spade"};
const string Card::rank_names[] = {"Unknown", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
