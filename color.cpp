#include "color.h" // ColorHandler

#ifdef _WIN32
    #include <windows.h>

    // special declaration for static variables
    WORD ColorHandler::Default;

    void ColorHandler::set_color(int type) {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        if (type == 1) // Heart or Diamond
            SetConsoleTextAttribute(hCon, 12 + 16*7); // f + 16*b, f=foreground, b=background
        else // Spade or Club
            SetConsoleTextAttribute(hCon, 0 + 16*7); // f + 16*b, f=foreground, b=background
    }

    bool ColorHandler::get_default() { // save the default color setting into the variable
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
            return false;
        ColorHandler::Default = info.wAttributes;
        return true;
    }

    void ColorHandler::set_default() { // restore the default color setting from the variable
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCon, ColorHandler::Default);
    }

#else
    #include <iostream> // cout
    using namespace std; // cout

    void ColorHandler::set_color(int type) {
        if (type == 1) // Heart or Diamond
            cout << "\033[1;31;47m";
        else // Spade or Club
            cout << "\033[30;47m";
    }

    bool ColorHandler::get_default() { // save the default color setting into the variable
        return true;
    }

    void ColorHandler::set_default() { // restore the default color setting from the variable
        cout << "\033[m";
    }

#endif
