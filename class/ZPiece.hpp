#pragma once
class ZPiece : public Piece {
public:
    ZPiece() {
        symbol = 'Z';
        shape[0][0] = shape[0][1] = symbol;
        shape[1][1] = shape[1][2] = symbol;
        rotationState = 0;
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        if (rotationState % 2 == 0) {
            shape[0][2] = shape[1][1] = shape[1][2] = shape[2][1] = symbol;
        } else {
            shape[0][0] = shape[0][1] = shape[1][1] = shape[1][2] = symbol;
        }
        rotationState = (rotationState + 1) % 2;
    }
};