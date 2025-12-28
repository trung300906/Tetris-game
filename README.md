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
│   ├── class.hpp \\ override source tree
│   ├── IPiece.hpp \\ IPiece class 
│   ├── JPiece.hpp \\ JPiece class
│   ├── LPiece.hpp \\ LPiece class
│   ├── OPiece.hpp \\ OPiece class
│   ├── Piece.hpp \\ BASIC class
│   ├── SPiece.hpp \\ SPiece class
│   ├── TPiece.hpp \\ TPiece class
│   └── ZPiece.hpp \\ZPiece
├── core.hpp \\ basic support, where link library vs basic variables
├── crossplatform.ipp \\ crossplatform function, support both linux vs windows
├── function_logic.ipp \\ function logic, where provide logic for games
├── LICENSE
├── README.md
└── tetris_main.cxx \\ main
```
4 directories, 16 files
## ABOUT MEMBERS
24521891 - TA DUY TRUNG
24520175 - NGUYEN TRAN GIA BAO
24521823 - VO LE UYEN TRANG
24521907 - HUYNH MINH TU
23521638 - LE CAO TRI 
