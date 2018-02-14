#include "card.h"
#include "color.h"

Card::Card () {
    my_suit = None;
    my_rank = Unknown;
}

Card::Card (Suit s, Rank r) {
    my_suit = s;
    my_rank = r;
}

Card::Suit Card::suit () const {
    return my_suit;
}

Card::Rank Card::rank () const {
    return my_rank;
}

void Card::set_suit (Suit s) {
    my_suit = s;
}

void Card::set_rank (Rank r) {
    my_rank = r;
}

//Return the suit's icon in unicode format.
wstring Card::suit_icon () const {
    if( my_suit == Club )
        return L"\u2663"; //5;
    else if( my_suit == Spade )
        return L"\u2660"; //6;
    else if( my_suit == Diamond )
        return L"\u2666"; //4;
    else if( my_suit == Heart )
        return L"\u2665"; //3;
    else
        return L"\u0000"; //0;
}

wstring Card::suit_name () const {
    return Suit_Names[my_suit];
}

wstring Card::rank_name () const {
    return Rank_Names[my_rank];
}

int Card::Points ( int people ) const {
    int score = 0;

    //Ace of spades
    if( suit_name() == L"Spade" && rank_name() == L"A" ){
        if( people == 3 || people == 4 )
            score += 30;
    }
    //Ace of clubs
    if( suit_name() == L"Club" && rank_name() == L"A" ){
        if( people == 4 )
            score += 40;
    }
    //red Ace
    if( rank_name() == L"A" ){
        if( suit_name() == L"Heart" || suit_name() == L"Diamond" )
            score += 20;
    }
    //red 9-King
    if( Card::Nine <= rank() && rank() <= Card::King ){
        if( suit_name() == L"Heart" || suit_name() == L"Diamond" )
            score += 10;
    }
    //red 2-8
    if( Card::Two <= rank() && rank() <= Card::Eight ){
        if( suit_name() == L"Heart" || suit_name() == L"Diamond" )
            score += static_cast<int>(rank());
    }
    return score;
}

//Use operator overloading to simplify coding.
void Card::operator =(const Card &card){
    set_suit(card.suit());
    set_rank(card.rank());
}
void Card::Print() const{
    if( suit_name() == L"Diamond" || suit_name() == L"Heart" )
        ColorHandler::SetColor(12,7);
    else
        ColorHandler::SetColor(0,7);
    wcout << suit_icon() << rank_name();
    ColorHandler::SetBackground();
}

const wstring Card::Suit_Names[] = { L"None", L"Club", L"Diamond", L"Heart", L"Spade" };
const wstring Card::Rank_Names[] = { L"Unknown", L"A", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K" };
