#include "core.hpp"
#include "crossplatform.ipp"
#include "class/class.hpp"
#include "function_logic.ipp"

int main() {
    srand(time(0));
    
#ifndef _WIN32
    setupTerminal();
#endif
   
    // Show high scores before starting game
    showHighScores();

    clearScreen();
    showCursor(false);
    initBoard();
    currentPiece = createRandomPiece();
    
    int frameCounter = 0;
    int dropSpeed = gameSpeed / 20; // Divide into smaller frames
    
    while (true) {
        // Handle input (non-blocking)
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
                    score += 1;
                }
            }
            else if (c == 'w' || c == 'W') {
                rotateWithWallKick();
            }
            else if (c == ' ') {
                hardDrop();
                frameCounter = dropSpeed; // Force immediate placement
            }
            else if (c == 'q' || c == 'Q') {
               goto end;
            }
#ifdef _WIN32
            else if (c == 0 || c == 224) {
                char k = getch();
                if (k == 75 && canMove(-1, 0)) x--; // Left
                if (k == 77 && canMove(1, 0)) x++;  // Right
                if (k == 80 && canMove(0, 1)) {     // Down
                    y++;
                    score += 1;
                }
                if (k == 72) rotateWithWallKick();  // Up
            }
#else
            else if (c == 27) {
                getch(); // Skip [
                char k = getch();
                if (k == 'D' && canMove(-1, 0)) x--;
                if (k == 'C' && canMove(1, 0)) x++;
                if (k == 'B' && canMove(0, 1)) {
                    y++;
                    score += 1;
                }
                if (k == 'A') rotateWithWallKick();
            }
#endif
            
            block2Board();
            draw();
        }
        
        // Auto drop logic
        frameCounter++;
        if (frameCounter >= dropSpeed) {
            frameCounter = 0;
            
            boardDelBlock();
            
            if (canMove(0, 1)) {
                y++;
            } else {
                block2Board();
                removeLine();

                // Tăng 1% tốc độ rơi mỗi khi block được đặt xuống
                gameSpeed = (int)(gameSpeed * 0.99);
                if (gameSpeed < 50) gameSpeed = 50; // Giới hạn tối thiểu
                dropSpeed = gameSpeed / 20;

                x = W / 2 - 2;
                y = 0;
                delete currentPiece;
                currentPiece = createRandomPiece();
                
                if (!canMove(0, 0)) {
                   goto end;
                }
            }
            
            block2Board();
            draw();
        }
        
        sleepMs(20); // Smooth 50 FPS
    }
end:   
    // Save score to file
    saveScore(score, level);
    cout << "\nScore saved!" << endl;

    block2Board();
    draw();
    gotoxy(0, H + 1);
    cout << "\n*** GAME OVER ***" << endl;
    cout << "Final Score: " << score << endl;
    cout << "Level Reached: " << level << endl;
    
    delete currentPiece;
    showCursor(true);
    
#ifndef _WIN32
    resetTerminal();
#endif
    
    return 0;
}