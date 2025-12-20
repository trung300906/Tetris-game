#pragma once
class JPiece : public Piece {
public:
    JPiece() {
        symbol = 'J';
        shape[0][0] = symbol;
        shape[1][0] = shape[1][1] = shape[1][2] = symbol;
        rotationState = 0;
    }
    
    void rotate() override {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
                
        switch (rotationState % 4) {
            case 0:
                shape[0][0] = symbol;
                shape[1][0] = shape[1][1] = shape[1][2] = symbol;
                break;
            case 1:
                shape[0][1] = shape[0][2] = shape[1][1] = shape[2][1] = symbol;
                break;
            case 2:
                shape[0][0] = shape[0][1] = shape[0][2] = shape[1][2] = symbol;
                break;
            case 3:
                shape[0][1] = shape[1][1] = shape[2][0] = shape[2][1] = symbol;
                break;
        }
        rotationState++;
    }
};