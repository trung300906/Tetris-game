# Tetris-game
Tetris Game source tree

## RULE OF CODE
about how to organize file source tree:
for sure make portable cross platform
file ipp should be inline function and code
file hpp should be just header where put class header or header of function

## SOURCE TREE:
``` plaintext
.
├── build
│   └── Debug
├── class
│   ├── class.hpp
│   ├── IPiece.hpp
│   ├── JPiece.hpp
│   ├── LPiece.hpp
│   ├── OPiece.hpp
│   ├── Piece.hpp
│   ├── SPiece.hpp
│   ├── TPiece.hpp
│   └── ZPiece.hpp
├── core.hpp
├── crossplatform.ipp
├── function_logic.ipp
├── LICENSE
├── README.md
├── tetris_main.cxx
└── tetris_main.run
```
4 directories, 16 files