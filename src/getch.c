#include <termios.h>
#include <stdio.h>
#include "getch.h"

static struct termios old, new;

void initTermios(int echo) {
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~ICANON;
    if (echo) {
        new.c_lflag |= ECHO;
    } else {
        new.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &new);
}

void resetTermios() {
    tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = (char) getchar();
    resetTermios();
    return ch;
}

char getch() {
    return getch_(0);
}

char getche() {
    return getch_(1);
}
