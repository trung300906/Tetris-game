#pragma once
class OPiece : public Piece {
public:
    OPiece() {
        symbol = 'O';
        shape[0][1] = shape[0][2] = shape[1][1] = shape[1][2] = symbol;
    }
    void rotate() override {}
};