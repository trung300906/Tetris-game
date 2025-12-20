#pragma once
void initBoard() {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            board[i][j] = (i == H - 1 || j == 0 || j == W - 1) ? '#' : ' ';
            boardColors[i][j] = (board[i][j] == '#') ? 9 : 0; // 9 là màu khung, 0 là ko màu
        }
    }
}

void drawUI() {
    gotoxy(W + 3, 1);
    cout << "TETRIS GAME";
    gotoxy(W + 3, 3);
    cout << "Score: " << score;
    gotoxy(W + 3, 4);
    cout << "Level: " << level;
    gotoxy(W + 3, 6);
    cout << "Controls:";
    gotoxy(W + 3, 7);
    cout << "A/Left  - Move Left";
    gotoxy(W + 3, 8);
    cout << "D/Right - Move Right";
    gotoxy(W + 3, 9);
    cout << "S/Down  - Soft Drop";
    gotoxy(W + 3, 10);
    cout << "W/Up    - Rotate";
    gotoxy(W + 3, 11);
    cout << "SPACE   - Hard Drop";
    gotoxy(W + 3, 12);
    cout << "Q       - Quit";
}

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            // Logic vẽ màu
            int colorToDraw = boardColors[i][j];
            
            // Nếu vị trí này trùng với Piece đang rơi, ưu tiên lấy màu của Piece
            if (currentPiece != nullptr) {
                 // Map toạ độ màn hình (i,j) ngược về toạ độ piece (ni, nj)
                 int ni = i - y;
                 int nj = j - x;
                 if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4) {
                     if (currentPiece->getCell(ni, nj) != ' ') {
                         colorToDraw = currentPiece->color;
                     }
                 }
            }

            // Thiết lập màu và vẽ
            setTextColor(colorToDraw);
            
            if (board[i][j] == '#') cout << "█"; 
            else if (currentPiece && i >= y && i < y+4 && j >= x && j < x+4 && currentPiece->getCell(i-y, j-x) != ' ') cout << "▓"; // Block đang rơi
            else if (board[i][j] != ' ') cout << "▓"; // Block đã đóng băng
            else cout << " ";
        }
        setTextColor(0); // Reset màu về mặc định để không bị lem màu ra UI
        cout << endl;
    }
    drawUI();
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ')
                if (y + i >= 0 && y + i < H && x + j >= 0 && x + j < W)
                    board[y + i][x + j] = ' ';
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ')
                if (y + i >= 0 && y + i < H && x + j >= 0 && x + j < W) {
                    board[y + i][x + j] = currentPiece->getCell(i, j);
                    boardColors[y + i][x + j] = currentPiece->color;
                }
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ') {
                int nx = x + j + dx;
                int ny = y + i + dy;
                if (nx <= 0 || nx >= W - 1 || ny >= H - 1 || ny < 0) return false;
                if (board[ny][nx] != ' ') return false;
            }
    return true;
}

void rotateWithWallKick() {
    char backup[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            backup[i][j] = currentPiece->getCell(i, j);
    
    int oldX = x;
    currentPiece->rotate();
    
    // Try original position
    if (canMove(0, 0)) return;
    
    // Try wall kicks
    int kicks[] = {-1, 1, -2, 2};
    for (int k : kicks) {
        x = oldX + k;
        if (canMove(0, 0)) return;
    }
    
    // Restore if rotation failed
    x = oldX;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentPiece->setCell(i, j, backup[i][j]);
}

void hardDrop() {
    boardDelBlock();
    while (canMove(0, 1)) {
        y++;
        score += 2; // Bonus points for hard drop
    }
}

void updateSpeed() {
    level = score / 100 + 1;
    gameSpeed = 500 - (level - 1) * 30;
    if (gameSpeed < 100) gameSpeed = 100;
}

void removeLine() {
    int linesCleared = 0;
    for (int i = H - 2; i > 0; i--) {
        int count = 0;
        for (int j = 1; j < W - 1; j++) if (board[i][j] != ' ') count++;
        
        if (count == W - 2) {
            linesCleared++;
            for (int k = i; k > 0; k--) {
                for (int j = 1; j < W - 1; j++) {
                    board[k][j] = board[k-1][j];
                    boardColors[k][j] = boardColors[k-1][j]; // Dời màu xuống
                }
            }
            // Xóa dòng trên cùng
            for (int j = 1; j < W - 1; j++) {
                board[0][j] = ' ';
                boardColors[0][j] = 0;
            }
            i++; 
        }
    }
    
    // Score calculation
    if (linesCleared > 0) {
        int points[] = {0, 40, 100, 300, 1200};
        score += points[linesCleared] * level;
        updateSpeed();
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