#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

#define H 21
#define W 16

char board[H][W];
int x = W / 2 - 2, y = 0;
int gameSpeed = 200;

class Piece {
protected:
    char shape[4][4];
    int rotationState;

public:
    Piece() : rotationState(0) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }

    virtual ~Piece() {}

    virtual void rotate() = 0;

    char getCell(int i, int j) const {
        return shape[i][j];
    }
};


class OPiece : public Piece {
public:
    OPiece() {
        shape[1][1] = shape[1][2] = shape[2][1] = shape[2][2] = 'O';
    }
    void rotate() override {

    }
};

class IPiece : public Piece {
public:
    IPiece() {
        shape[0][1] = shape[1][1] = shape[2][1] = shape[3][1] = 'I';
    }
    void rotate() override {

    }
};

class TPiece : public Piece {
public:
    TPiece() {
        shape[1][1] = 'T';
        shape[2][0] = shape[2][1] = shape[2][2] = 'T';
    }
    void rotate() override {
    }
};

class LPiece : public Piece {
public:
    LPiece() {
        shape[1][2] = shape[2][0] = shape[2][1] = shape[2][2] = 'L';
        rotationState = 0;
    }

    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';

        switch (rotationState) {
        case 0:
            shape[1][2] = shape[2][0] = shape[2][1] = shape[2][2] = 'L';
            break;
        case 1:
            shape[0][1] = shape[1][1] = shape[2][1] = shape[2][2] = 'L';
            break;
        case 2:
            shape[1][0] = shape[1][1] = shape[1][2] = shape[2][0] = 'L';
            break;
        case 3:
            shape[0][0] = shape[0][1] = shape[1][1] = shape[2][1] = 'L';
            break;
        }

        rotationState = (rotationState + 1) % 4;
    }
};

Piece* currentPiece = nullptr;

void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void showCursor(bool show) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = show;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            board[i][j] = (i == H - 1 || j == 0 || j == W - 1) ? '#' : ' ';
}

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++)
            cout << board[i][j];
        cout << endl;
    }
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ')
                board[y + i][x + j] = ' ';
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ')
                board[y + i][x + j] = currentPiece->getCell(i, j);
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ') {
                int nx = x + j + dx;
                int ny = y + i + dy;
                if (nx <= 0 || nx >= W - 1 || ny >= H - 1) return false;
                if (board[ny][nx] != ' ') return false;
            }
    return true;
}

void rotateCurrentBlock() {
    currentPiece->rotate();
}

Piece* randomPiece() {
    int r = rand() % 4;
    switch (r) {
    case 0: return new IPiece();
    case 1: return new OPiece();
    case 2: return new TPiece();
    default: return new LPiece();
    }
}

int main() {
    srand(time(0));
    showCursor(false);
    initBoard();

    currentPiece = randomPiece();

    while (true) {
        boardDelBlock();

        if (_kbhit()) {
            int c = _getch();
            if (c == 'a' || c == 'A') {
                if (canMove(-1, 0)) x--;
            }
            else if (c == 'd' || c == 'D') {
                if (canMove(1, 0)) x++;
            }
            else if (c == 's' || c == 'S') {
                if (canMove(0, 1)) y++;
            }
            else if (c == 'w' || c == 'W') {
                rotateCurrentBlock();
            }
            else if (c == 0 || c == 224) {
                char k = _getch();
                if (k == 75 && canMove(-1, 0)) x--;
                if (k == 77 && canMove(1, 0))  x++;
                if (k == 80 && canMove(0, 1))  y++;
                if (k == 72) rotateCurrentBlock();
            }
}


        if (canMove(0, 1)) y++;
        else {
            block2Board();
            x = W / 2 - 2;
            y = 0;

            delete currentPiece;
            currentPiece = randomPiece();

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
