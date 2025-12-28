#include "core.hpp"
#include "crossplatform.ipp"
#include "class/class.hpp"
#include "function_logic.ipp"

void playGame() {
    score = 0;
    level = 1;
    gameSpeed = 500;
    x = 4; y = 0;
    initBoard();
    if (currentPiece) {
        delete currentPiece;
        currentPiece = nullptr;
    }
    if (nextPiece) { 
        delete nextPiece; 
        nextPiece = nullptr; 
    }
    currentPiece = getNextPiece();
    
    clearScreen();
    showCursor(false);
    
    int frameCounter = 0;
    // Tăng 3% tốc độ rơi mỗi khi block được đặt xuống
    gameSpeed = (int)(gameSpeed * 0.97);
    if (gameSpeed < 50) gameSpeed = 50; // Giới hạn tối thiểu
    int dropSpeed = gameSpeed / 20;

    while (true) {
        while (kbhit()) {
            int c = getch();
            
            boardDelBlock();
            
            if (c == 'a' || c == 'A') {
                if (canMove(-1, 0)) x--;
            }
            else if (c == 'd' || c == 'D') {
                if (canMove(1, 0)) x++;
            }
            else if (c == 's' || c == 'S') {
                if (canMove(0, 1)) {
                    y++;
                }
            }
            else if (c == 'w' || c == 'W') {
                rotateWithWallKick();
            }
            else if (c == ' ') {
                hardDrop();
                frameCounter = dropSpeed;
            }
            else if (c == 'q' || c == 'Q') {
               goto end;
            }
            #ifdef _WIN32
            else if (c == 0 || c == 224) {
                char k = getch();
                if (k == 75 && canMove(-1, 0)) x--; // Left
                if (k == 77 && canMove(1, 0)) x++;  // Right
                if (k == 80 && canMove(0, 1)) { y++; } // Down
                if (k == 72) rotateWithWallKick();  // Up
            }
            #else
            else if (c == 27) {
                getch();
                char k = getch();
                if (k == 'D' && canMove(-1, 0)) x--;
                if (k == 'C' && canMove(1, 0)) x++;
                if (k == 'B' && canMove(0, 1)) { y++; }
                if (k == 'A') rotateWithWallKick();
            }
            #endif
            
            block2Board();
            draw();
        }
        
        frameCounter++;
        if (frameCounter >= dropSpeed) {
            frameCounter = 0;
            boardDelBlock();
            
            if (canMove(0, 1)) {
                y++;
            } else {
                block2Board();
                removeLine();
                dropSpeed = gameSpeed / 20;

                // Tạo block mới
                x = W / 2 - 2; y = 0;
                delete currentPiece;
                currentPiece = getNextPiece();
                
                // Kiểm tra thua game ngay khi vừa tạo block mới
                if (!canMove(0, 0)) {
                   goto end;
                }
            }
            
            block2Board();
            draw();
        }
        
        sleepMs(20); 
    }
end:   
    // Save score to file
    saveScore(score, level);
    cout << "\nScore saved!" << endl;

    block2Board();
    draw();
    
    // Vẽ khung thông báo Game Over
    setTextColor(3);
    gotoxy(W + 3, 14); cout << "=== GAME OVER ===";
    setTextColor(0);
    
    gotoxy(W + 3, 15); cout << "Final Score: " << score;
    gotoxy(W + 3, 17); cout << "Press any key";
    gotoxy(W + 3, 18); cout << "to return menu...";
    
    while(kbhit()) getch();
    getch();
}

int main() {
    srand(time(0));
    #ifdef _WIN32
    setupWindowsConsole();
    #else
    setupTerminal();
    #endif

    while (true) {
        clearScreen();
        showCursor(true);
        setTextColor(3);
        cout << "=========================" << endl;
        cout << "   SUPER TETRIS C++      " << endl;
        cout << "=========================" << endl;
        setTextColor(0); // Reset màu
        
        cout << "1. Play Game" << endl;
        cout << "2. High Scores" << endl;
        cout << "3. Exit" << endl;
        cout << "\nChoose option (1-3): ";
        cout.flush(); // Đảm bảo hiển thị ngay

        // Đợi input hợp lệ
        char choice;
        while (true) {
            // Chỉ dùng getch() - terminal đã được setup sẵn từ main()
            choice = getch();
            if (choice == '1' || choice == '2' || choice == '3') {
                break;
            }
        }
        
        if (choice == '1') {
            playGame();
        } 
        else if (choice == '2') {
            showHighScores();
        }
        else if (choice == '3') {
            clearScreen();
            cout << "\nGoodbye!" << endl;
            break;
        }
    }

    #ifndef _WIN32
    resetTerminal();
    #endif
    return 0;
}