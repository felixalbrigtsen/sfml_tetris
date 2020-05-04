#ifndef TETRIS_H
#define TETRIS_H

#include <array>

#define BOARDWIDTH 10
#define BOARDHEIGHT 20

const int BOARDSIZE = BOARDWIDTH*BOARDHEIGHT;

//According to the official SRS system
const uint_fast8_t TETROMINOES[7][4][4][2] = { //[Shape][Rotation][Block][0=x 1=y]
    {   /* I */
        { {0,1}, {1,1}, {2,1}, {3,1} },
        { {2,0}, {2,1}, {2,2}, {2,3} },
        { {0,2}, {1,2}, {2,2}, {3,2} },
        { {1,0}, {1,1}, {1,2}, {1,3} }
    },
    {   /* J */
        { {0,0}, {0,1}, {1,1}, {2,1} },
        { {2,0}, {1,0}, {1,1}, {1,2} },
        { {0,1}, {1,1}, {2,1}, {2,2} },
        { {1,0}, {1,1}, {1,2}, {0,2} }
    },
    {   /* L */
        { {0,1}, {1,1}, {2,1}, {2,0} },
        { {0,0}, {1,0}, {1,1}, {1,2} },
        { {0,2}, {0,1}, {1,1}, {2,1} },
        { {1,0}, {1,1}, {1,2}, {2,2} }
    },
    {   /* Square */
        { {1,0}, {1,1}, {2,0}, {2,1} },
        { {1,0}, {1,1}, {2,0}, {2,1} },
        { {1,0}, {1,1}, {2,0}, {2,1} },
        { {1,0}, {1,1}, {2,0}, {2,1} }
    },
    {   /* S */
        { {0,1}, {1,1}, {1,0}, {2,0} },
        { {1,0}, {1,1}, {2,1}, {2,2} },
        { {0,2}, {1,2}, {1,1}, {2,1} },
        { {0,0}, {0,1}, {1,1}, {1,2} }
    },
    {   /* Z */
        { {0,0}, {1,0}, {1,1}, {2,1} },
        { {2,0}, {2,1}, {1,1}, {1,2} },
        { {0,1}, {1,1}, {1,2}, {2,2} },
        { {1,0}, {1,1}, {0,1}, {0,2} }
    },
    {   /* T */
        { {0,1}, {1,1}, {2,1}, {1,0} },
        { {1,0}, {1,1}, {1,2}, {2,1} },
        { {0,1}, {1,1}, {2,1}, {1,2} },
        { {1,0}, {1,1}, {1,2}, {0,1} }
    },
};


class Tetris {
    private:
        //std::array<std::array<char, BOARDWIDTH>, BOARDHEIGHT> staticBoard;
        std::array<int, BOARDSIZE> staticBoard;

        int curPiece;
        int curx;
        int cury;
        int curr;
        int nextPiece;

        int lines;
        int points;
        int level;
        int levelProgress;

        bool gameOver;

        //Functions
        void newPiece();
        bool testLanded();
        void clearRows();
        void lock();
        void rotate();
        bool move(bool right, bool left);
        bool fall();
        
    public:
        std::array<int, BOARDSIZE> getBoard();
        void tick(bool keys[], bool fall);
        void init();
        std::array<int, 4> getInfo();
};

#endif