#ifndef COLOR_H
#define COLOR_H

class ColorHandler {
    public:
        static void SetColor(int f,int b);
        static bool GetBackground();
        static void SetBackground();

    private:
        static unsigned short background;
};

#endif // COLOR_H
