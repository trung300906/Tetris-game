#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>
    #include <sys/ioctl.h>
#endif

using namespace std;

#define H 22
#define W 12

char board[H][W] = {};
int boardColors[H][W] = {};
int gameSpeed = 500;
int score = 0;
int level = 1;

class Piece;
class IPiece;
class OPiece;
class TPiece;
class SPiece;
class ZPiece;
class JPiece;
class LPiece;
struct GameRecord;
// Game variables
int x = 4, y = 0;
Piece* currentPiece = nullptr;
Piece* nextPiece = nullptr;