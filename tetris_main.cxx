#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

#define H 21
#define W 16

char board[H][W] = {};
char curShape[4][4];
int x = 4, y = 0;
int gameSpeed = 200;

class Piece {
protected:
    char shape[4][4];
public:
    Piece() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }
    virtual ~Piece() {}
    virtual void rotate() = 0;

    void copyTo(char dest[4][4]) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                dest[i][j] = shape[i][j];
    }
};

class SPiece : public Piece {
public:
    SPiece() {
        shape[1][1] = shape[1][2] = 'S';
        shape[2][0] = shape[2][1] = 'S';
    }

    void rotate() override {
        char tmp[4][4] = {};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp[j][3 - i] = shape[i][j];

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = tmp[i][j];
    }
};

Piece* currentPiece = nullptr;


void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void showCursor(bool show) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(out, &info);
    info.bVisible = show;
    SetConsoleCursorInfo(out, &info);
}


void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            board[i][j] = (i == H - 1 || j == 0 || j == W - 1) ? '#' : ' ';
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (curShape[i][j] != ' ')
                board[y + i][x + j] = ' ';
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (curShape[i][j] != ' ')
                board[y + i][x + j] = curShape[i][j];
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (curShape[i][j] != ' ') {
                int tx = x + j + dx;
                int ty = y + i + dy;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1) return false;
                if (board[ty][tx] != ' ') return false;
            }
    return true;
}

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++)
            cout << board[i][j];
        cout << endl;
    }
}

int main() {
    srand(time(0));
    showCursor(false);
    initBoard();

    currentPiece = new SPiece();
    currentPiece->copyTo(curShape);

    while (1) {
        boardDelBlock();

        if (_kbhit()) {
            int c = _getch();
            if (c == 224) {
                c = _getch();
                if (c == 75 && canMove(-1, 0)) x--;
                if (c == 77 && canMove(1, 0))  x++;
                if (c == 80 && canMove(0, 1))  y++;
                if (c == 72) {
                    currentPiece->rotate();
                    currentPiece->copyTo(curShape);
                }
            }
        }

        if (canMove(0, 1)) y++;
        else {
            block2Board();
            x = W / 2 - 2;
            y = 0;

            delete currentPiece;
            currentPiece = new SPiece();
            currentPiece->copyTo(curShape);

            if (!canMove(0, 0)) {
                gotoxy(0, H + 1);
                cout << "GAME OVER!";
                break;
            }
        }

        block2Board();
        draw();
        Sleep(gameSpeed);
    }

    delete currentPiece;
    return 0;
}
