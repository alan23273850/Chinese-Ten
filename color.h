#ifndef COLOR_H
#define COLOR_H

class ColorHandler {
    public:
        static void set_color(int type);
        static bool get_default();
        static void set_default();

    private:
        static unsigned short Default;
};

#endif // COLOR_H
