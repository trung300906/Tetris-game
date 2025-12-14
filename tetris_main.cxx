#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

#define H 21
#define W 16

char board[H][W] = {};
int gameSpeed = 200;

// Class cơ sở cho tất cả các khối Tetris
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
        // Khối O không đổi khi xoay
    }
};

// Khối T - xoay 4 trạng thái
class TPiece : public Piece {
public:
    TPiece() {
        shape[1][1] = 'T';
        shape[2][0] = shape[2][1] = shape[2][2] = 'T';
        rotationState = 0;
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        switch (rotationState) {
            case 0: // ┴
                shape[1][1] = shape[2][0] = shape[2][1] = shape[2][2] = 'T';
                break;
            case 1: // ├
                shape[0][1] = shape[1][1] = shape[1][2] = shape[2][1] = 'T';
                break;
            case 2: // ┬
                shape[1][0] = shape[1][1] = shape[1][2] = shape[2][1] = 'T';
                break;
            case 3: // ┤
                shape[0][1] = shape[1][0] = shape[1][1] = shape[2][1] = 'T';
                break;
        }
        rotationState = (rotationState + 1) % 4;
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

// Khối J - xoay 4 trạng thái
class JPiece : public Piece {
public:
    TPiece() {
        shape[1][1] = 'T';
        shape[2][0] = shape[2][1] = shape[2][2] = 'T';
    }
    void rotate() override {
    }
};

// Khối L - xoay 4 trạng thái
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

// Game variables
int x = 4, y = 0;
Piece* currentPiece = nullptr;

void gotoxy(int x, int y) {
    COORD c = { x, y };
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

bool canRotate() {
    // Lưu trạng thái hiện tại
    char backup[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            backup[i][j] = currentPiece->getCell(i, j);
    
    // Thử xoay
    currentPiece->rotate();
    
    // Kiểm tra va chạm tại vị trí hiện tại
    bool canRotateHere = true;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentPiece->getCell(i, j) != ' ') {
                int tx = x + j;
                int ty = y + i;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1 || (ty >= 0 && board[ty][tx] != ' ')) {
                    canRotateHere = false;
                    break;
                }
            }
        }
        if (!canRotateHere) break;
    }
    
    if (canRotateHere) return true;
    
    // Thử wall kick (dịch phải)
    x++;
    canRotateHere = true;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentPiece->getCell(i, j) != ' ') {
                int tx = x + j;
                int ty = y + i;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1 || (ty >= 0 && board[ty][tx] != ' ')) {
                    canRotateHere = false;
                    break;
                }
            }
        }
        if (!canRotateHere) break;
    }
    
    if (canRotateHere) return true;
    
    // Thử wall kick (dịch trái)
    x -= 2;
    canRotateHere = true;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentPiece->getCell(i, j) != ' ') {
                int tx = x + j;
                int ty = y + i;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1 || (ty >= 0 && board[ty][tx] != ' ')) {
                    canRotateHere = false;
                    break;
                }
            }
        }
        if (!canRotateHere) break;
    }
    
    if (canRotateHere) return true;
    
    // Không xoay được, khôi phục
    x++;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentPiece->setCell(i, j, backup[i][j]);
    
    return false;
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
        } else {
            updateSpeed();
        }
    }
}

Piece* createRandomPiece() {
    int type = rand() % 7;
    switch (type) {
        case 0: return new IPiece();
        case 1: return new OPiece();
        case 2: return new TPiece();
        case 3: return new SPiece();
        case 4: return new ZPiece();
        case 5: return new JPiece();
        case 6: return new LPiece();
        default: return new OPiece();
    }
}

int main() {
    srand(time(0));
    showCursor(false);
    initBoard();
    currentPiece = createRandomPiece();
    
    while (1) {
        boardDelBlock();
        
        if (_kbhit()) {
            int c = _getch();
            if (c == 0 || c == 224) {
                char arrow = _getch();
                switch (arrow) {
                case 75: // Left
                    if (canMove(-1, 0)) x--;
                    break;
                case 77: // Right
                    if (canMove(1, 0)) x++;
                    break;
                case 80: // Down
                    if (canMove(0, 1)) y++;
                    break;
                case 72: // Up - Rotate
                    canRotate();
                    break;
                }
            } else {
                if ((c == 'a' || c == 'A') && canMove(-1, 0)) x--;
                if ((c == 'd' || c == 'D') && canMove(1, 0)) x++;
                if ((c == 's' || c == 'S') && canMove(0, 1)) y++;
                if (c == 'w' || c == 'W') canRotate();
                if (c == 'q' || c == 'Q') break;
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


        if (canMove(0, 1))
            y++;
        else {
            block2Board();
            removeLine();
            x = W / 2 - 2;
            y = 0;
            delete currentPiece;
            currentPiece = createRandomPiece();
            if (!canMove(0, 0)) {
                gotoxy(0, H + 2);
                cout << "GAME OVER!" << endl;
                delete currentPiece;
                break;
            }
        }
        
        block2Board();
        draw();
        Sleep(gameSpeed);
    }
    
    return 0;
}