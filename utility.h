#ifndef UTILITY_H
#define UTILITY_H

#include <vector> // vector
#include <ctime> // time

using namespace std; // vector

const int COMPUTER_KEY_TIME = 800;

string int_to_char(int n);
void _sleep(int ms);
void clear_screen();
int Getch();
int Getche();
int Getintfromch();
int Getintfromche();
void Getline(istream& _Istr, string& _Str);
void init_termios(int echo);
void reset_original_termios();

template<class T>
void selection_sort(vector<T> &vec, bool cmp(const T&, const T&))
{
    // In each stage we assume the elements in the interval [0..i-1]
    // are properly sorted, and we want to sort in [i..N-1]. After
    // this stage, we hope [i] is the minimum in [i..N-1].
	for (int i=0; i+1<vec.size(); i++) {
		T min = vec.at(i);
        int index = i;
		for (int j=i+1; j<vec.size(); j++)
			if (cmp(vec.at(j), min)) {
				min = vec.at(j);
				index = j;
			}
        // Now we have already found the minimum element [index].
        // Then we're going to move it to the location [i].
        // The easiest way is to swap the two elements directly.
		vec.at(index) = vec.at(i);
		vec.at(i) = min;
	}
}

template<class T>
void bubble_sort(vector<T> &vec, int start, int end, bool cmp(const T&, const T&))
{
    bool swapped;
    do {
        swapped = false;
        for (int i=start; i<end; i++)
            if (cmp(vec.at(i+1), vec.at(i))) { // if [i] > [i+1], then swap them.
                T temp = vec.at(i);
                vec.at(i) = vec.at(i+1);
                vec.at(i+1) = temp;
                swapped = true;
            }
    } while (swapped);
}

template<class T>
void bubble_sort(vector<T> &vec, bool cmp(const T&, const T&)) {
    bubble_sort(vec, 0, (int)vec.size()-1, cmp);
}

template<class T>
void shuffle(vector<T> &vec)
{
    // Duplicate the vector to the buffer and set the order = {0, 1, 2, ..., N-1}.
    vector<int> order(vec.size());
    for (int i=0; i<vec.size(); i++)
        order.at(i) = i;

    // Rearrange (randomize) the vector "order."
    srand(time(NULL));
    for (int i=0; i+1<vec.size(); i++) {
        int j = i + (rand() % (vec.size() - i));
        int temp = order.at(i);
        order.at(i) = order.at(j);
        order.at(j) = temp;
    }

    // Shuffle the vector based on the randomized order derived above.
    vector<T> buffer = vec;
    for (int i=0; i<vec.size(); i++)
        vec.at(order.at(i)) = buffer.at(i);
}

#endif // UTILITY_H
