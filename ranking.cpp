#include <iostream> // cout
#include <string> // getline
#include <fstream> // FILE
#include <iomanip> // setw
#include "utility.h" // sort functions, clear_screen
#include "ranking.h" // Ranking

using namespace std; // cout

void Ranking::recording() {
    ofstream FILE;
    switch (records.size()) {
        case 2: FILE.open(Ranking::file_names[0], ios::app); break;
        case 3: FILE.open(Ranking::file_names[1], ios::app); break;
        case 4: FILE.open(Ranking::file_names[2], ios::app); break;
        default: return;
    }
    for (int i=0; i<records.size(); i++)
        FILE << records.at(i).name << ',' << records.at(i).score << endl;
    FILE.close();
}

void Ranking::print() {
    clear_screen();
    cout << "==================== Ranking Board =====================\n\n";
    cout << "=Rank=|============== Name ==============|=== Points ===\n";
    for (int i=0; i<records.size(); i++) {
        if (i < 99)
            cout << "  " << setfill('0') << setw(2) << i+1 << "  |" << setfill(' ');
        else
            cout << " " << setfill('0') << setw(3) << i+1 << "  |" << setfill(' ');
        cout << setw(33) << records.at(i).name << " |";
        cout << setw(5) << records.at(i).score << " point(s)\n";
    }
}

void Ranking::print(int rows) {
    clear_screen();
    cout << "==================== Ranking Board =====================\n\n";
    cout << "=Rank=|============== Name ==============|=== Points ===\n";
    int i;
    for (i=0; i<(rows < records.size() ? rows : records.size()); i++) {
        cout << "  " << setfill('0') << setw(2) << i+1 << "  |" << setfill(' ');
        cout << setw(33) << records.at(i).name << " |";
        cout << setw(5) << records.at(i).score << " point(s)\n";
    }
    for (; i<rows; i++) {
        cout << "  " << setfill('0') << setw(2) << i+1 << "  |" << setfill(' ');
        for (int j=0; j<34; j++) cout << '-'; cout << '|';
        for (int j=0; j<14; j++) cout << '-'; cout << endl;
    }
}

void Ranking::read_file(int player_num)
{
    // Clear the original data.
    records.clear();

    // Open the file.
    ifstream FILE;
    switch (player_num) {
        case 2: FILE.open(Ranking::file_names[0]); break;
        case 3: FILE.open(Ranking::file_names[1]); break;
        case 4: FILE.open(Ranking::file_names[2]); break;
        default: return;
    }
    if (!FILE) return;

    // Read in.
    int score;
    string name;
    while (getline(FILE, name, ',')) {
        FILE >> score;
        records.push_back(Score(name, score));
        getline(FILE, name); // absorb the remaining buffer to the EOL. "name" in this line is meaningless.
    }

    // Close the file.
    FILE.close();
}

void Ranking::sort_by_score()
{
    // First sort by score in "descending" order with selection sort.
    selection_sort(records, Score::cmp_by_score);

    // If there are same scores, sort them by name in "ascending" order with bubble sort.
    int end=0, start;
    while (end+1 < records.size()) { // make "end" run in [0..N-2]
        start = end;
        while (end+1 < records.size() // Use index boundary check to avoid error.
            && records.at(start).score == records.at(end+1).score)
            end++;
        // So far, we ensure the elements in [start..end] have the same score. Then
        // we want to perform bubble sort in this interval. Even if start==end, the
        // result is correct because no sorting is executed.
        bubble_sort(records, start, end, Score::cmp_by_name);

        // So far, we guarantee [end] and [end+1] must have different scores, so we can
        // definitely increment our base pointer and disregard all things before [end+1].
        end++;
    }
}

void Ranking::sort_by_name()
{
    // First sort by name in "ascending" order with selection sort.
    selection_sort(records, Score::cmp_by_name);

    // If there are same names, sort them by score in "descending" order with bubble sort.
    int end=0, start;
    while (end+1 < records.size()) { // make "end" run in [0..N-2]
        start = end;
        while (end+1 < records.size() // Use index boundary check to avoid error.
            && records.at(start).name == records.at(end+1).name)
            end++;
        // So far, we ensure the elements in [start..end] have the same name. Then
        // we want to perform bubble sort in this interval. Even if start==end, the
        // result is correct because no sorting is executed.
        bubble_sort(records, start, end, Score::cmp_by_score);

        // So far, we guarantee [end] and [end+1] must have different names, so we can
        // definitely increment our base pointer and disregard all things before [end+1].
        end++;
    }
}

bool Ranking::extract_by_name(string search) // Find someone's name by binary search.
{
    int left = 0;
    int right = (int)records.size()-1;
    int mid, start, end;
    bool found = false;
    while (left <= right) { // Note that <= cannot be replaced by <.
        mid = (left + right) / 2;
        string name = records.at(mid).name; // mid must be calculated first
        if (search < name)
            right = mid - 1;
        else if (search > name)
            left = mid + 1;
        else { // (search == name)
            found = true;
            start = end = mid;
            while (start-1>=left && records.at(start-1).name==search)
				start--; // Look for the leftmost score of the same player.
            while (end+1<=right && records.at(end+1).name==search)
				end++; // Look for the rightmost score of the same player.
            break;
        }
    }
    if (!found) return false; // If nothing is found, terminate the function.

    records = vector<Score>(records.begin()+start, records.begin()+end+1);
    return true;
}

const string Ranking::file_names[] = {"ranking_2_players.csv", "ranking_3_players.csv", "ranking_4_players.csv"};
