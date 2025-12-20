#pragma once
class Piece {
protected:
    char shape[4][4];
    int rotationState;
    char symbol;

public:
    Piece() : rotationState(0), symbol('X') {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }

    virtual ~Piece() {}
    virtual void rotate() = 0;

    char getCell(int i, int j) const {
        return shape[i][j];
    }

    void setCell(int i, int j, char c) {
        shape[i][j] = c;
    }
};