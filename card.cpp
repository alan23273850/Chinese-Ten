#include "card.h"

void SetColor(int f,int b)//Used to generate poker-like colors
{
    unsigned short ForeColor=f+16*b;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,ForeColor);
}

        Card::Card () {
            my_suit = None;
            my_rank = Unknown;
        }

        Card::Card(Suit s, Rank r) {
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

        //Return the suit's icon in ASCII.
        char Card::suit_icon () const {
            if( my_suit == Club )
                return 5;
            else if( my_suit == Spade )
                return 6;
            else if( my_suit == Diamond )
                return 4;
            else if( my_suit == Heart )
                return 3;
            else
                return 0;
        }

        string Card::suit_name () const {
            return Suit_Names[my_suit];
        }

        string Card::rank_name () const {
            return Rank_Names[my_rank];
        }

        int Card::Points ( int people ) const {

            int score = 0;
            //Ace of spades
            if( suit_name()=="Spade" && rank_name()=="A" ){
                if( people == 3 || people == 4 )
                    score += 30;
                else
                    score += 0;
            }
            //Ace of clubs
            if( suit_name()=="Club" && rank_name()=="A" ){
                if( people == 4 )
                    score += 40;
                else
                    score += 0;
            }
            //red Ace
            if( rank_name()=="A" ){
                if( suit_name()=="Heart" || suit_name()=="Diamond" )
                    score += 20;
                else
                    score += 0;
            }
            //red 9-King
            if( Card::Nine<=rank() && rank()<=Card::King ){
                if( suit_name()=="Heart" || suit_name()=="Diamond" )
                    score += 10;
                else
                    score += 0;
            }
            //red 2-8
            if( Card::Two<=rank() && rank()<=Card::Eight ){
                if( suit_name()=="Heart" || suit_name()=="Diamond" )
                    score += static_cast<int>(rank());
                else
                    score += 0;
            }
            return score;
        }

        //Use operator overloading to simplify coding.
        void Card::operator =(const Card &card){
            set_suit(card.suit());
            set_rank(card.rank());
        }
        void Card::Print() const{
            if( suit_name() == "Diamond" || suit_name() == "Heart" )
                SetColor(12
                         ,7);
            else
                SetColor(0,7);
            cout << suit_icon() << rank_name();
            SetColor();
        }

const string Card::Suit_Names[] = { "None", "Club", "Diamond", "Heart", "Spade" };
const string Card::Rank_Names[] = { "Unknown", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

