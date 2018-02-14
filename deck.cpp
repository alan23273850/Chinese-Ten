#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <stdlib.h>
#include "deck.h"
#include "color.h"

using namespace std;

wchar_t int_to_char (int n) {
    if( 0<=n && n<=9 )
        return (wchar_t)('0'+n);
    else if( 10<=n && n<=35 )
        return (wchar_t)('A'+n-10);
    else
        return (wchar_t)0;
}

Deck::Deck () {} // Constructor, create an empty deck.

void Deck::Full () { // Constructor-like function, create a full deck.
    cards.clear();
    for (int i=1; i<=13; i++) {
        for (int j=1; j<=4; j++) {
            Card card(static_cast<Card::Suit>(j), static_cast<Card::Rank>(i));
            cards.push_back(card);
        }
    }
    /*
    //Generate special cases for homework 6-2.
    for (int j=0; j<4; j++){
        Card card;
        card = cards.at(48+j);
        cards.at(48+j) = cards.at(24+j);
        cards.at(24+j) = card;
    }

    //Generate special cases for homework 6-2.
    Card card;
    card = cards.at(11);
    cards.at(11) = cards.at(24);
    cards.at(24) = card;*/
}

void Deck::Shuffle () { // randomize the sequence of the cards
    //Use clock to initialize the seed.
    srand((unsigned)time(NULL));

    //Replicate a deck to a buffer and set order = {0,1,2,...,50,51}.
    vector<Card> cards_buffer(52);
    vector<int> order(52);
    int random, temp;
    for(int i=0; i<52; i++){
        order.at(i) = i;
        cards_buffer.at(i) = cards.at(i);
    }

    //Rearrange(randomize) the array "order."
    for(int i=0; i<52; i++){
        random = rand() % 52;
        temp = order.at(i);
        order.at(i) = order.at(random);
        order.at(random) = temp;
    }

    //Put the cards in the buffer into the original deck based on the randomized order.
    for(int i=0; i<52; i++)
        cards.at(i) = cards_buffer.at(order.at(i));
}

void Deck::Clear () {
    cards.clear();
}

Card &Deck::Deal () { // hand out the next card on top
    Card &card = cards.at(cards.size()-1);
    cards.pop_back();
    return card;
}

void Deck::Add (const Card& card) { // Add a new card.
    cards.push_back(card);
}

void Deck::Delete (const int &index) { // Delete a card based on the index.
    cards.erase(cards.begin()+index);
}

size_t Deck::number () const { // Return the number of cards in the deck.
    return cards.size();
}

Card &Deck::card (size_t i) { // Return the content of the card based on the index.
    return cards.at(i);
}

void Deck::Sort () { // Use bubble sorting to sort the deck. Rank in ascending order. Suit in descending order.
    bool swapped = false;
    //Sort the rank.
    do{
        swapped = false;
        for(int i=0; i<static_cast<int>(cards.size())-1; i++){
            if(cards.at(i).rank() > cards.at(i+1).rank()){
                Card temp;
                temp = cards.at(i);
                cards.at(i) = cards.at(i+1);
                cards.at(i+1) = temp;
                swapped = true;
            }
        }
    }while(swapped == true);

    //Sort the suit.
    int index=0, index_before, index_after;
    while(index < static_cast<int>(cards.size())-1){
        index_before = index;
        while(index < static_cast<int>(cards.size())-1 && cards.at(index).rank() == cards.at(index+1).rank())//Use index boundary check to prevent from error.
            index++;
        index_after = index;
        do{
            swapped = false;
            for(int i=index_before; i<index_after; i++){
                if(cards.at(i).suit() < cards.at(i+1).suit()){
                    Card temp;
                    temp = cards.at(i);
                    cards.at(i) = cards.at(i+1);
                    cards.at(i+1) = temp;
                    swapped = true;
                }
            }
        }while(swapped == true);
        index++;
    }
}

void Deck::Print () { // print all the cards in the deck
    Sort();
    char order;
    wcout << ' ';
    for(unsigned int i=0; i<cards.size(); i++){//Print the order.
        if(i<=9)
            order = (char)('0' + i);
        else
            order = (char)('A' + i - 10);
        wcout << '(' << order << ')';
        wcout << ' ';
    }
    wcout << endl;

    wcout << ' ';
    for(unsigned int i=0; i<cards.size(); i++){//Print the suit.
        if(cards.at(i).suit_name() == L"Diamond" || cards.at(i).suit_name() == L"Heart")
            ColorHandler::SetColor(12,7);
        else
            ColorHandler::SetColor(0,7);
        wcout << setw(2) << cards.at(i).suit_icon() << ' ';
        ColorHandler::SetBackground();
        wcout << ' ';
    }
    wcout << endl;

    wcout << ' ';
    for(unsigned int i=0; i<cards.size(); i++){//Print the rank.
        if(cards.at(i).suit_name() == L"Diamond" || cards.at(i).suit_name() == L"Heart")
            ColorHandler::SetColor(12,7);
        else
            ColorHandler::SetColor(0,7);
        wcout << setw(2) << cards.at(i).rank_name() << ' ';
        ColorHandler::SetBackground();
        wcout << ' ';
    }
    wcout << endl;
}

Card::Rank Deck::special_check (int n) const {//n=3(or 4), check three(or four) same specific cards
    int five=0, ten=0, jack=0, queen=0, king=0;
    for(unsigned int i=0; i<cards.size(); i++){
        switch(cards.at(i).rank()){
            case Card::Five: five++; break;
            case Card::Ten: ten++; break;
            case Card::Jack: jack++; break;
            case Card::Queen: queen++; break;
            case Card::King: king++; break;
            default: break;
        }
    }
    if( five == n )
        return Card::Five;
    else if( ten == n )
        return Card::Ten;
    else if( jack == n )
        return Card::Jack;
    else if( queen == n )
        return Card::Queen;
    else if( king == n )
        return Card::King;
    else
        return Card::Unknown;
}

//Use operator overloading to simplify coding.
void Deck::operator =(const Deck &deck) {
    cards.resize(deck.number());
    for(int i=0; i<(int)deck.number(); i++)
        cards.at(i) = deck.cards.at(i);
}

vector<pair<int, int>> Find_match(Deck deck1, Deck deck2) {
    vector<pair<int, int>> result;
    pair<int, int> iter;
    for(int i=0; i<(int)deck1.number(); i++){
        for(int j=0; j<(int)deck2.number(); j++){
            if( 1<=static_cast<int>(deck1.card(i).rank()) && static_cast<int>(deck1.card(i).rank())<=9 ){
                if( static_cast<int>(deck1.card(i).rank()) + static_cast<int>(deck2.card(j).rank()) == 10 ){
                    iter.first = i;
                    iter.second = j;
                    result.push_back(iter);
                }
            }
            else if( static_cast<int>(deck1.card(i).rank())==10
                  || static_cast<int>(deck1.card(i).rank())==11
                  || static_cast<int>(deck1.card(i).rank())==12
                  || static_cast<int>(deck1.card(i).rank())==13 ){
                if( static_cast<int>(deck1.card(i).rank()) == static_cast<int>(deck2.card(j).rank()) ){
                    iter.first = i;
                    iter.second = j;
                    result.push_back(iter);
                }
            }
            else{
            }
        }
    }
    return result;
}

bool Print_match_first(Deck deck1, Deck deck2, vector<pair<int, int>> Pair) {
    if( Pair.size()>0 )
        wcout << L"\nHere is a quick search for you.\n\n";
    else
        return false;
    for(unsigned int i=0; i<Pair.size(); i++){
        wcout << L"You can use your card (" << int_to_char(Pair.at(i).first) << ')';
        deck1.card(Pair.at(i).first).Print();
        wcout << L" to capture card (" << int_to_char(Pair.at(i).second) << ')';
        deck2.card(Pair.at(i).second).Print();
        wcout << L".\n";
    }
    return true;
}

int Print_match_second(Deck deck, vector<pair<int, int>> Pair) {
    if( Pair.size()>0 ){
        wcout << L"\nHere is a quick search for you.\n\n";
        wcout << L"You can capture card";
    }
    else
        return 0;
    for(unsigned int i=0; i<Pair.size(); i++){
        wcout << ' ';
        wcout << '(' << int_to_char(Pair.at(i).second) << ')';
        deck.card(Pair.at(i).second).Print();
        if( i<Pair.size()-1 )
            wcout << ',';
        else
            wcout << L".\n";
    }
    return 1;
}
