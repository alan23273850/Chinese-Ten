#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include "ranking.h"

using namespace std;
const int MAX_LENGTH=10000;

Ranking::Ranking(){
    players.clear();
}

Score Ranking::GetPlayer(int i){
    return players.at(i);
}

int Ranking::Recording(int people){
    ofstream FILE;
    switch(people){
        case 2: FILE.open("rec_2_players.txt",ios::app); break;
        case 3: FILE.open("rec_3_players.txt",ios::app); break;
        case 4: FILE.open("rec_4_players.txt",ios::app); break;
        default: return 0;
    }
    for(unsigned int i=0; i<players.size(); i++)
        FILE << players.at(i).name << '|' << players.at(i).score << endl;
    FILE.close();
    return 1;
}

void Ranking::Push_back(Score player){
    players.push_back(player);
}

void Ranking::Pop_back(){
    players.pop_back();
}

void Ranking::Clear(){
    players.clear();
}

void Ranking::Print(unsigned int n){

    cout << "=====================Ranking Board======================\n\n";
    cout << "=Rank=|===============Name===============|====Points====" << endl;
    unsigned int i;
    for(i=0; i<players.size(); i++){
        printf("  %02d  |",i+1);
        cout << setw(34) << players.at(i).name << '|';
        cout << setw(5) << players.at(i).score;
        cout << " point(s)\n";
    }
    while(i<n){

        printf("  %02d  |",i+1);
        for(int j=0; j<34; j++)
            cout << '-';
        cout << '|';
        for(int j=0; j<14; j++)
            cout << '-';
        cout << endl;
        i++;
    }
}

void Ranking::SortByScore(){// Sort by score

    Score iter;
    unsigned int index, index_before, index_after;
    bool swapped=false;
    int max;
    //Sort by score first with insertion sort.
    for(unsigned int i=0; i<players.size()-1; i++){
        max = players.at(i).score;
        index = i;
        for(unsigned int j=i+1; j<players.size(); j++){
            if( players.at(j).score > max ){
                max = players.at(j).score;
                index = j;
            }
        }
        iter = players.at(index);
        for(unsigned int j=index; j>i; j--)
            players.at(j) = players.at(j-1);
        players.at(i) = iter;        
    }
    //If there are same scores, sort them by name with bubble sort.
    index=0;
    while( index < players.size()-1 ){
        index_before = index;
        while( index<players.size()-1 && players.at(index).score == players.at(index+1).score )//Use index boundary check to prevent from error.
            index++;
        index_after = index;
        do{
            swapped = false;
            for(unsigned int i=index_before; i<index_after; i++){
                if( players.at(i).name > players.at(i+1).name ){
                    Score temp;
                    temp = players.at(i);
                    players.at(i) = players.at(i+1);
                    players.at(i+1) = temp;
                    swapped = true;
                }
            }
        }while( swapped == true );
        index++;
    }
}

void Ranking::SortByName(){
    Score iter;
    string min;
    unsigned int index, index_before, index_after;
    bool swapped = false;

    //Sort by name first with selection sort.
    for(unsigned int i=0; i<players.size()-1; i++){
        index = i;
        min = players.at(i).name;        
        for(unsigned int j=i+1; j<players.size(); j++){
            if( players.at(j).name < min ){
                min = players.at(j).name;
                index = j;
            }            
        }
        iter = players.at(index);        
        for(unsigned int j=index; j>i; j--)
            players.at(j) = players.at(j-1);
        players.at(i) = iter;        
    }

    //If there are same names, sort them by score with bubble sort.
    index=0;
    while( index < players.size()-1 ){
        index_before = index;
        while( index<players.size()-1 && players.at(index).name == players.at(index+1).name )//Use index boundary check to prevent from error.
            index++;
        index_after = index;
        do{
            swapped = false;
            for(unsigned int i=index_before; i<index_after; i++){
                if( players.at(i).score < players.at(i+1).score ){
                    Score temp;
                    temp = players.at(i);
                    players.at(i) = players.at(i+1);
                    players.at(i+1) = temp;
                    swapped = true;
                }
            }
        }while( swapped == true );
        index++;
    }
}

int Ranking::FindByName( int play_persons, string search ){//Find someone's name by binary search.
    ifstream FILE;
    Ranking record;
    string name;
    Score iter;
    string cpp_str;
    unsigned index, index_left, index_right, index_iter;
    bool found = false;
    char c_str[100];

    //Clear the original data in the object.
    Clear();

    //Open the file.
    switch(play_persons){
        case 2: FILE.open("rec_2_players.txt"); if( !FILE ) return 0; break;
        case 3: FILE.open("rec_3_players.txt"); if( !FILE ) return 0; break;
        case 4: FILE.open("rec_4_players.txt"); if( !FILE ) return 0; break;
        default: return 0;
    }

    //Read in.
    while(FILE.getline(c_str,MAX_LENGTH,'|')){
        cpp_str.assign(c_str);        
        iter.name = cpp_str;        
        FILE.getline(c_str,MAX_LENGTH,'\n');
        iter.score = atoi(c_str);
        record.Push_back(iter);
    }
    FILE.close();

    //Sort first.
    record.SortByName();

    //Binary search
    index_left = 0;
    index_right = record.players.size()-1;
    while( found==false && index_left<=index_right ){//Note that <= cannot be replaced by <.
        index = (index_left+index_right)/2;
        name = record.players.at(index).name;
        if( name == search )
            found = true;
        else if( name < search )
            index_left = index+1;
        else if( name > search )
            index_right = index-1;
        else
            break;
    }

    //If nothing is found, terminate the function.
    if( found==false )
        return 0;

    //Look for other same player's score.
    index_iter = index;
    while( index_iter>0 && record.players.at(index_iter-1).name==record.players.at(index_iter).name )
        index_iter--;
    do{
        Push_back(record.players.at(index_iter));
        index_iter++;
    }while( index_iter<record.players.size() && record.players.at(index_iter-1).name==record.players.at(index_iter).name );

    return 1;
}

int Ranking::FindTop(int play_persons, unsigned int top_persons){
    ifstream FILE;
    Score iter;
    string cpp_str;
    char c_str[100];

    //Clear the original data in the object.
    Clear();

    //Open the file.
    switch(play_persons){
        case 2: FILE.open("rec_2_players.txt"); if( !FILE ) return 0; break;
        case 3: FILE.open("rec_3_players.txt"); if( !FILE ) return 0; break;
        case 4: FILE.open("rec_4_players.txt"); if( !FILE ) return 0; break;
        default: return 0;
    }

    //Read in.
    while(FILE.getline(c_str,MAX_LENGTH,'|')){
        cpp_str.assign(c_str);
        iter.name = cpp_str;
        FILE.getline(c_str,MAX_LENGTH,'\n');
        iter.score = atoi(c_str);
        Push_back(iter);
    }
    FILE.close();

    //Insertion sort
    SortByScore();

    //Store only top_persons elements in players.
    while( players.size()>top_persons )
        players.pop_back();

    return 1;
}
