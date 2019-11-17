#define SLEEP

#include <iostream> // istream
#include <string> // getline
#ifdef _WIN32
    #include <windows.h> // Sleep
    #include <conio.h> // _getch, _getche
#else
    #include <unistd.h> // sleep (UNIX)
    #include <termios.h> // termios
#endif

using namespace std; // getline

#ifdef __linux__
    /* Reference: https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux */

    static struct termios current, original, game;

    /* Initialize new terminal i/o settings */
    void change_termios(int echo, termios *before, termios &after) {
        tcgetattr(0, before); /* grab old terminal i/o settings */
        after = *before; /* make new settings same as old settings */
        after.c_lflag &= ~ICANON; /* disable buffered i/o */
        if (echo)
            after.c_lflag |= ECHO; /* set echo mode */
        else
            after.c_lflag &= ~ECHO; /* set no echo mode */
        tcsetattr(0, TCSANOW, &after); /* use these new terminal i/o settings now */
    }
    void init_termios(int echo) {
        change_termios(echo, &original, game);
    }

    void set_termios(int echo) {
        change_termios(echo, &game, current);
    }

    /* Restore old terminal i/o settings */
    void reset_termios(termios *termios) {
        tcsetattr(0, TCSANOW, termios);
    }
    void reset_original_termios() {
        tcsetattr(0, TCSANOW, &original);
    }

#endif

string int_to_char(int n) {
    if (0 <= n&&n <= 9)
        return string(1, '0' + n);
    else if (10 <= n&&n <= 35)
        return string(1, 'A' + n - 10);
    else if (36 <= n&&n <= 61)
        return "A" + string(1, 'A' + n - 36);
    else
        return "";
}

int char_to_int(char ch) {
    if ('0' <= ch&&ch <= '9')
        return ch - '0';
    else if ('a' <= ch&&ch <= 'z')
        return ch - 'a' + 10;
    else if ('A' <= ch&&ch <= 'Z')
        return ch - 'A' + 10;
    else
        return -1;
}

void _sleep(int ms) {
    #ifdef SLEEP
        #ifdef _WIN32
            Sleep(ms);
        #else
            usleep(1000 * ms);
        #endif
    #endif
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        cout << "\033[1J\033[1;1H";
    #endif
}

void clear_input() {
    #ifdef _WIN32
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    #else
        tcflush(STDIN_FILENO, TCIFLUSH);
    #endif
}

int Getch() { // a wrapper to prevent players from inputing characters at an inappropriate time
    clear_input();
    #ifdef _WIN32
        return _getch();
    #else
        set_termios(0);
        int ch = getchar();
        reset_termios(&game);
        return ch;
    #endif
}

int Getche() { // a wrapper to prevent players from inputing characters at an inappropriate time
    clear_input();
    #ifdef _WIN32
        return _getche();
    #else
        set_termios(1);
        int ch = getchar();
        reset_termios(&game);
        return ch;
    #endif
}

void Getline(istream& _Istr, string& _Str) { // a wrapper to prevent players from inputing characters at an inappropriate time
    clear_input();
    #ifdef _WIN32
        getline(_Istr, _Str);
    #else
        reset_original_termios();
        getline(_Istr, _Str);
        reset_termios(&game);
    #endif
}

int Getintfromch() { // convert input char into the proper integer.
    char key = Getch();
    return char_to_int(key);
}

int Getintfromche() { // convert input char into the proper integer.
    char key = Getche();
    return char_to_int(key);
}
