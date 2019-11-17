#ifndef RANKING_H
#define RANKING_H

#include <iostream> // string
#include <vector> // vector

using namespace std; // string

struct Score {
    string name;
    int score;

    Score(string _name, int _score) { name = _name; score = _score; }
    static bool cmp_by_score(const Score &s1, const Score &s2) { return s1.score > s2.score; }
    static bool cmp_by_name(const Score &s1, const Score &s2) { return s1.name < s2.name; }
    void operator=(const Score &right) { name = right.name; score = right.score; }
};

class Ranking {
    public:
        Score record(int i) { return records.at(i); }
        int size() { return records.size(); }
        void clear() { records.clear(); }
        void push_back(Score record) { records.push_back(record); }
        void recording();
        void print();
        void print(int rows);
        void read_file(int player_num);
        void sort_by_score();
        void sort_by_name();
        bool extract_by_name(string search);
        void operator=(const Ranking &right) { records = right.records; }

    private:
        vector<Score> records;
        static const string file_names[];
};

#endif // RANKING_H
