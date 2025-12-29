#pragma once
#ifdef _WIN32
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
void setupWindowsConsole() {
    // Setup UTF-8 cho console
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Bật xử lý ANSI escape sequences (Windows 10+)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else
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

void setTextColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int winColor = 7;
    switch(color) {
        case 0: winColor = 7; break; // Trắng
        case 1: winColor = 12; break; // Đỏ
        case 2: winColor = 10; break; // Xanh lá
        case 3: winColor = 14; break; // Vàng
        case 4: winColor = 9; break;  // Xanh dương
        case 5: winColor = 13; break; // Tím
        case 6: winColor = 11; break; // Cyan
        case 9: winColor = 8; break;  // Màu xám cho khung
    }
    SetConsoleTextAttribute(hConsole, winColor);
#else
    switch(color) {
        case 0: printf("\033[0m"); break;   // Reset
        case 1: printf("\033[1;31m"); break; // Red
        case 2: printf("\033[1;32m"); break; // Green
        case 3: printf("\033[1;33m"); break; // Yellow
        case 4: printf("\033[1;34m"); break; // Blue
        case 5: printf("\033[1;35m"); break; // Magenta
        case 6: printf("\033[1;36m"); break; // Cyan
        case 9: printf("\033[1;30m"); break; // Grey
    }
#endif
}
