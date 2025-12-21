#pragma once

vector<GameRecord> loadScores() {
    vector<GameRecord> records;
    ifstream file("tetris_scores.txt");
    
    if (file.is_open()) {
        GameRecord record;
        while (file >> record.score >> record.level) {
            file.ignore(); // Skip space
            getline(file, record.date);
            records.push_back(record);
        }
        file.close();
    }
    
    return records;
}

void saveScore(int finalScore, int finalLevel) {
    vector<GameRecord> records = loadScores();
    
    GameRecord newRecord;
    newRecord.score = finalScore;
    newRecord.level = finalLevel;
    
    // Get current time
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timeStr[100];
    sprintf(timeStr, "%02d/%02d/%04d %02d:%02d", 
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900,
            ltm->tm_hour, ltm->tm_min);
    newRecord.date = timeStr;
    
    records.push_back(newRecord);
    
    // Sort by score (descending)
    sort(records.begin(), records.end(), [](const GameRecord& a, const GameRecord& b) {
        return a.score > b.score;
    });
    
    // Keep only top 5
    if (records.size() > 5) {
        records.resize(5);
    }
    
    // Save to file
    ofstream file("tetris_scores.txt");
    if (file.is_open()) {
        for (const auto& record : records) {
            file << record.score << " " << record.level << " " << record.date << endl;
        }
        file.close();
    }
}


void showHighScores() {
    vector<GameRecord> records = loadScores();
    
    clearScreen();
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════╗\n";
    cout << "  ║           TETRIS - HIGH SCORES                 ║\n";
    cout << "  ╚════════════════════════════════════════════════╝\n\n";

    if (records.empty()) {
        cout << "  No previous games recorded.\n";
    } else {
        int highestScore = records[0].score;
        cout << "  Highest Score: " << highestScore << " (Level " << records[0].level << ")\n\n";
        cout << "  Top 5 Scores:\n";
        cout << "  ┌────┬─────────┬───────┬─────────────────────┐\n";
        cout << "  │ #  │  Score  │ Level │        Date         │\n";
        cout << "  ├────┼─────────┼───────┼─────────────────────┤\n";
        
        for (size_t i = 0; i < records.size(); i++) {
            printf("  │ %2zu │ %7d │  %2d   │ %-19s │\n", 
                   i + 1, records[i].score, records[i].level, records[i].date.c_str());
        }
        
        cout << "  └────┴─────────┴───────┴─────────────────────┘\n";
    }
    
    cout << "\n  Press any key to start game...";
    // Dùng cin.get() cho cả Windows và Linux
    sleepMs(5000); // Đảm bảo terminal đã sẵn sàng nhận input
    cin.get();
#ifdef _WIN32
    _getch();
#else
    setupTerminal();
    getch();
    resetTerminal();
#endif
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            board[i][j] = (i == H - 1 || j == 0 || j == W - 1) ? '#' : ' ';
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
            if (board[i][j] == '#') {
                cout << "█";
            } else if (board[i][j] == ' ') {
                cout << " ";
            } else {
                cout << "▓";
            }
        }
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
                if (y + i >= 0 && y + i < H && x + j >= 0 && x + j < W)
                    board[y + i][x + j] = currentPiece->getCell(i, j);
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
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] != ' ') count++;
        }
        
        if (count == W - 2) {
            linesCleared++;
            // Move all lines above down
            for (int k = i; k > 0; k--) {
                for (int j = 1; j < W - 1; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            // Clear top line
            for (int j = 1; j < W - 1; j++) {
                board[0][j] = ' ';
            }
            i++; // Check this line again
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