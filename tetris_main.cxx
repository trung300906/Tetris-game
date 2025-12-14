#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;

#define H 21
#define W 16

char board[H][W] = {};

char blocks[][4][4] = {
    {{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '}},
    {{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'I','I','I','I'},{' ',' ',' ',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','T',' ',' '},{'T','T','T',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','S','S',' '},{'S','S',' ',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'Z','Z',' ',' '},{' ','Z','Z',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'J',' ',' ',' '},{'J','J','J',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ',' ','L',' '},{'L','L','L',' '},{' ',' ',' ',' '}}
};

int x = 4, y = 0, b = 1; 
int gameSpeed = 200;

class Piece {
protected:
    char shape[4][4];
    int rotationState;
public:
    Piece() : rotationState(0) { for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) shape[i][j] = ' '; }
    virtual ~Piece() {}
    void rotateClockwise() {
        char temp[4][4];
        const int N = 4;
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                temp[c][N - 1 - r] = shape[r][c];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                shape[i][j] = temp[i][j];
    }
    virtual void rotate() = 0;
    char getCell(int i, int j) const { return shape[i][j]; }
};

class RotatablePiece : public Piece {
public:
    void rotate() override {
        this->rotateClockwise(); 
        rotationState = (rotationState + 1) % 4;
    }
};

class TPiece : public RotatablePiece {
public:
    TPiece() {
        shape[1][1] = 'T';
        shape[2][0] = shape[2][1] = shape[2][2] = 'T'; 
        rotationState = 0;
    }
};

class JPiece : public RotatablePiece {
public:
    JPiece() {
        shape[1][0] = shape[2][0] = shape[2][1] = shape[2][2] = 'J';
        rotationState = 0;
    }
};

Piece* currentPiece = nullptr; 

Piece* createRandomPiece() {
    int type = rand() % 2; 
    if (type == 0) return new TPiece();
    else return new JPiece();
}

void gotoxy(int x, int y) {
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void showCursor(bool show) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = show;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void boardDelBlock() {
    if (!currentPiece) return;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ' && y + i < H && x + j < W)
                board[y + i][x + j] = ' ';
}

void block2Board() {
    if (!currentPiece) return;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ' && y + i < H && x + j < W)
                board[y + i][x + j] = currentPiece->getCell(i, j);
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if ((i == H - 1) || (j == 0) || (j == W - 1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << "Speed: " << (250 - gameSpeed) << " ";
}

bool canMove(int dx, int dy) {
    if (!currentPiece) return false;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ') {
                int tx = x + j + dx;
                int ty = y + i + dy;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1) return false;
                if (ty >= 0 && board[ty][tx] != ' ') return false;
            }
    return true;
}

void updateSpeed() {
    gameSpeed -= 15;
    if (gameSpeed < 50) gameSpeed = 50;
}

void removeLine() {
    int k = H - 2;
    for (int i = H - 2; i > 0; i--) {
        int count = 0;
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] != ' ') count++;
            board[k][j] = board[i][j];
        }

        if (count < W - 2) {
            k--;
        }
        else {
            updateSpeed();
        }
    }
}

void rotateCurrentBlock() { 
    currentPiece->rotate();
}


int main()
{
    srand(time(0));
    showCursor(false);
    currentPiece = createRandomPiece();
    initBoard();
    while (1) {
        boardDelBlock();
        if (_kbhit()) {
            int c = _getch();
            if (c == 0 || c == 224) {
                char arrow = _getch();
                switch (arrow) {
                case 75:
                    if (canMove(-1, 0)) x--;
                    break;
                case 77:
                    if (canMove(1, 0)) x++;
                    break;
                case 80:
                    if (canMove(0, 1)) y++;
                    break;
                case 72:
                    rotateCurrentBlock(); 
                    break;
                }
            }
            else {
                if ((c == 'a' || c == 'A') && canMove(-1, 0)) x--;
                if ((c == 'd' || c == 'D') && canMove(1, 0)) x++;
                if ((c == 's' || c == 'S') && canMove(0, 1)) y++;
                if (c == 'q' || c == 'Q') break;
            }
        }

        if (canMove(0, 1))
            y++;
        else {
            block2Board();
            removeLine();
            x = W / 2 - 2;
            y = 0;
            
            delete currentPiece;
            currentPiece = createRandomPiece(); 
            b = rand() % 7; 
            
            if (!canMove(0, 0)) {
                gotoxy(0, H + 2);
                cout << "GAME OVER!";
                break;
            }
        }
        block2Board();
        draw();
        Sleep(gameSpeed);
    }
    return 0;
}