#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
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
    int rotationState;
    
public:
    Piece() : rotationState(0) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }
    
    virtual ~Piece() {}
    
    // Phương thức ảo thuần túy - bắt buộc các lớp con phải implement
    virtual void rotate() = 0;
    
    // Phương thức xoay cơ bản (clockwise 90 degrees)
    void rotateClockwise() {
        char temp[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[j][3 - i] = shape[i][j];
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }
    
    char getCell(int i, int j) const {
        return shape[i][j];
    }
    
    void setCell(int i, int j, char c) {
        shape[i][j] = c;
    }
    
    int getRotationState() const { return rotationState; }
};

// Khối I - xoay 2 trạng thái (ngang/dọc)
class IPiece : public Piece {
public:
    IPiece() {
        shape[0][1] = shape[1][1] = shape[2][1] = shape[3][1] = 'I';
    }
    
    void rotate() override {
        if (rotationState == 0) {
            // Dọc -> Ngang
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    shape[i][j] = ' ';
            shape[1][0] = shape[1][1] = shape[1][2] = shape[1][3] = 'I';
            rotationState = 1;
        } else {
            // Ngang -> Dọc
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    shape[i][j] = ' ';
            shape[0][1] = shape[1][1] = shape[2][1] = shape[3][1] = 'I';
            rotationState = 0;
        }
    }
};

// Khối O - không xoay
class OPiece : public Piece {
public:
    OPiece() {
        shape[1][1] = shape[1][2] = shape[2][1] = shape[2][2] = 'O';
    }
    
    void rotate() override {
        // Khối O không đổi khi xoay
    }
};
class TPiece : public RotatablePiece {
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

// Khối S - xoay 2 trạng thái
class SPiece : public Piece {
public:
    SPiece() {
        shape[1][1] = shape[1][2] = shape[2][0] = shape[2][1] = 'S';
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        if (rotationState == 0) {
            shape[0][1] = shape[1][1] = shape[1][2] = shape[2][2] = 'S';
            rotationState = 1;
        } else {
            shape[1][1] = shape[1][2] = shape[2][0] = shape[2][1] = 'S';
            rotationState = 0;
        }
    }
};

// Khối Z - xoay 2 trạng thái
class ZPiece : public Piece {
public:
    ZPiece() {
        shape[1][0] = shape[1][1] = shape[2][1] = shape[2][2] = 'Z';
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        if (rotationState == 0) {
            shape[0][2] = shape[1][1] = shape[1][2] = shape[2][1] = 'Z';
            rotationState = 1;
        } else {
            shape[1][0] = shape[1][1] = shape[2][1] = shape[2][2] = 'Z';
            rotationState = 0;
        }
    }
};

class JPiece : public Piece {
public:
    JPiece() {
        shape[1][0] = shape[2][0] = shape[2][1] = shape[2][2] = 'J';
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        switch (rotationState) {
            case 0:
                shape[1][0] = shape[2][0] = shape[2][1] = shape[2][2] = 'J';
                break;
            case 1:
                shape[0][1] = shape[0][2] = shape[1][1] = shape[2][1] = 'J';
                break;
            case 2:
                shape[1][0] = shape[1][1] = shape[1][2] = shape[2][2] = 'J';
                break;
            case 3:
                shape[0][1] = shape[1][1] = shape[2][0] = shape[2][1] = 'J';
                break;
        }
        rotationState = (rotationState + 1) % 4;
    }
};

class LPiece : public Piece {
public:
    LPiece() {
        shape[1][2] = shape[2][0] = shape[2][1] = shape[2][2] = 'L';
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
int x = 4, y = 0;
Piece* currentPiece = nullptr; 

Piece* createRandomPiece() {
    int type = rand() % 2; 
    if (type == 0) return new TPiece();
    else return new JPiece();
}

void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
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
    cout << "Speed: " << (250 - gameSpeed) << " | Controls: Arrow Keys/WASD | Q: Quit   ";
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

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++)
            cout << board[i][j];
        cout << endl;
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

    currentPiece = new SPiece();
    currentPiece->copyTo(curShape);

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
        }

        if (canMove(0, 1)) y++;
        else {
            block2Board();
            x = W / 2 - 2;
            y = 0;
            
            delete currentPiece;
            currentPiece = createRandomPiece(); 
            b = rand() % 7; 
            
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
