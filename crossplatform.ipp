#pragma once
// Cross-platform functions
#ifndef _WIN32
struct termios orig_termios;

void setupTerminal() {
    tcgetattr(0, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &raw);
}

void resetTerminal() {
    tcsetattr(0, TCSANOW, &orig_termios);
}

int kbhit() {
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

int getch() {
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    }
    return c;
}
#endif

void gotoxy(int x, int y) {
#ifdef _WIN32
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
#endif
}

void showCursor(bool show) {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = show;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
#else
    if (show)
        printf("\e[?25h");
    else
        printf("\e[?25l");
    fflush(stdout);
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[1;1H");
    fflush(stdout);
#endif
}

void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}