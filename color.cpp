#include <windows.h>
#include "color.h"

// special declaration for static variables
unsigned short ColorHandler::background;

void ColorHandler::SetColor(int f, int b)
{
    unsigned short ForeColor = (unsigned short)(f+16*b);
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,ForeColor);
}

bool ColorHandler::GetBackground(){
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return false;
    ColorHandler::background = info.wAttributes;
    return true;
}

void ColorHandler::SetBackground()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, ColorHandler::background);
}