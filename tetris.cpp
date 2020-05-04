//Felix Albrigtsen 2020
//All game logic, but no graphics are defined in this file

#include "tetris.hpp"
#include <array>
#include <stdlib.h>
#include <time.h>
#include <iostream>

void Tetris::newPiece() {
    //Initialize a new piece at the top, and prepare the next.
    cury = 0;
    curx = (BOARDWIDTH / 2)-1;
    curr = 0;
    
    curPiece = nextPiece;
    nextPiece = rand() % 7;

    //If the piece being placed exceeds the top of the board
    if (staticBoard[BOARDWIDTH*1.5] != 0) { gameOver = true; }
}

void Tetris::init() {
    srand((unsigned) time(NULL));
    staticBoard.fill(0);
    newPiece();
    lines = 0;
    points = 0;
    level = 0;
    levelProgress = 0;
    nextPiece = rand() % 7;
}

std::array<int, BOARDSIZE> Tetris::getBoard() {
    //Returns staticboard, with the current piece superimposed on it
    std::array<int, BOARDSIZE> board = staticBoard;
    for (int i = 0; i < 4; i++) {
        int xpos = curx + TETROMINOES[curPiece][curr][i][0];
        int ypos = cury + TETROMINOES[curPiece][curr][i][1];
        int index = (ypos * BOARDWIDTH) + xpos;
        board[index] = curPiece+1;
    }
    return board;
}

std::array<int, 4> Tetris::getInfo() {
    //Return Level, Lines, Score, Next piece
    std::array<int, 4> info = {level, lines, points, nextPiece };
    return info;
}

void Tetris::clearRows() {
    //Scan the lines for full rows. Remove filled lines and give points
    int prevlines = lines;

    for (int line = 0; line < BOARDHEIGHT; line++) {
        bool full = true;
        for (int i = 0; i < BOARDWIDTH; i++) {
            if (staticBoard[ (line*BOARDWIDTH) + i ] == 0) {
                full = false;
            }
        }
        if (full) {

            lines += 1;
            levelProgress += 1;
            if (levelProgress > level) {
                level += 1;
                levelProgress = 0;
            }
            
            //Move everything down one line:
            int startIndex = (line*BOARDWIDTH) - 1;
            for (int i = startIndex; i > 0; i--) {
                staticBoard[i + BOARDWIDTH] = staticBoard[i];
            }

        }
    }
    //Scoring according to the Tetris wikia
    switch (lines - prevlines) {
        case 1:
            points += 40 * (level+1);
            break;
        case 2:
            points += 100 * (level+1);
            break;
        case 3:
            points += 300 * (level+1);
            break;
        case 4:
            points += 1200 * (level+1);
            break;
    }
}

bool Tetris::testLanded() {
    //Returns true if a part of the tetromino is directly above another piece
    for (int i = 0; i < 4; i++) {
        int xpos = curx + TETROMINOES[curPiece][curr][i][0];
        int ypos = cury + TETROMINOES[curPiece][curr][i][1] + 1;
        int index = (ypos * BOARDWIDTH) + xpos;
        if (staticBoard[index] != 0 || ypos == BOARDHEIGHT) {
            return true;
        }
    }
    return false;
}

void Tetris::lock() {
    //Places the current piece onto the staticboard
    for (int i = 0; i < 4; i++) {
        int xpos = curx + TETROMINOES[curPiece][curr][i][0];
        int ypos = cury + TETROMINOES[curPiece][curr][i][1];
        int index = (ypos * BOARDWIDTH) + xpos;
        staticBoard[index] = curPiece+1;
    }
}

bool Tetris::move(bool left, bool right) {
    //Returns true if the move was legal
    int newx;
    if (right && !left) {
        newx = curx + 1;
    } else if (!right && left) {
        newx = curx - 1;
    } else {
        return true;
    }

    bool possible = true;
    for (int i = 0; i < 4; i++) {
        int xpos = newx + TETROMINOES[curPiece][curr][i][0];
        int ypos = cury + TETROMINOES[curPiece][curr][i][1];
        int index = (ypos * BOARDWIDTH) + xpos;
        if (staticBoard[index] != 0) { possible = false; }
        if (xpos < 0 || xpos >= BOARDWIDTH) { possible = false; }
    }
    if (possible) { 
        curx = newx;
    }
    return possible;
}

void Tetris::rotate() {
    //Try to rotate 
    int newr = (curr == 3) ? 0 : curr + 1; //Add one, wrap 4 -> 0
    bool possible = true;
    bool kickleft = false;
    for (int i = 0; i < 4; i++) {
        int xpos = curx + TETROMINOES[curPiece][newr][i][0];
        int ypos = cury + TETROMINOES[curPiece][newr][i][1];
        int index = (ypos * BOARDWIDTH) + xpos;
        if (staticBoard[index] != 0 || xpos >= BOARDWIDTH || xpos < 0) {
            possible = false;
        }
    } 
    if (possible) {
        curr = newr;
    } else {
        //If normal rotation is not possible, try to kick off the edge
        int oldr = curr;
        curr = newr;
        bool hasMoved = false;

        hasMoved |= move(true, false);
        if (!hasMoved) {
            hasMoved |= move(false, true);
        }
        if (!hasMoved) { //No kicking was possible
            curr = oldr;
        }

    }
}

void Tetris::tick(bool keys[], bool fallfast) {
    //Main Game Loop
    //Takes these inputs:
    //  keys[] = boolean, true if these keys are held down, in order: left, right, up, down, space
    //  fall = boolean, true if this is a "gravity tick". Otherwise, only right/left and rotation is performed

    if (gameOver) { return; }

    //Left/Right
    move(keys[0], keys[1]);

    //Rotate
    if (keys[2]) { rotate(); }
    
    //Instant fall
    if (keys[4]) {
        bool landed = testLanded();
        while (!landed) {
            landed = testLanded();
            if (!landed) { cury += 1; }
        }
    }

    if (fallfast || keys[3]) { 
        bool landed = testLanded(); 
        if (landed) {
            lock();
            clearRows();
            newPiece();
        } else {
            cury += 1;
        }
    }
}
