#pragma once
class SPiece : public Piece {
public:
    SPiece() {
        symbol = 'S';
        shape[0][1] = shape[0][2] = symbol;
        shape[1][0] = shape[1][1] = symbol;
        rotationState = 0;
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        if (rotationState % 2 == 0) {
            shape[0][1] = shape[1][1] = shape[1][2] = shape[2][2] = symbol;
        } else {
            shape[0][1] = shape[0][2] = shape[1][0] = shape[1][1] = symbol;
        }
        rotationState = (rotationState + 1) % 2;
    }
};