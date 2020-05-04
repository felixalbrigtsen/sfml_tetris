//Felix Albrigtsen 2020
//Graphics and loops are contained in this file.
//All game logic is defined in tetris.cpp

#include "tetris.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <math.h>

#define SCALE 50
#define MENUWIDTH SCALE*BOARDWIDTH
#define MENUMARGIN 80
#define MENUTEXT 30
#define NPIECESIZE (MENUWIDTH-(2*MENUMARGIN))

#define MOVES_PER_TICK 2

int target_delay = 650; //Miliseconds between each fall

sf::Color colors[] = {
    sf::Color::Black,
    sf::Color::Cyan,
    sf::Color::Blue,
    sf::Color(255,128,0),
    sf::Color::Yellow,
    sf::Color::Green,
    sf::Color::Magenta,
    sf::Color::Red
};

sf::RenderWindow window(sf::VideoMode(BOARDWIDTH*SCALE + MENUWIDTH, BOARDHEIGHT*SCALE),"Tetris");
sf::Font font;
sf::Text txt_level;
sf::Text txt_lines;
sf::Text txt_score;
sf::Text txt_next;
sf::RectangleShape nextSquare;
sf::RectangleShape NPieceRect;

static int NPieceBlockSize = NPIECESIZE / 6;

void drawBoard(std::array<int, BOARDSIZE> board) {
    for (int i = 0; i < BOARDSIZE; i++) {
        int x = i % BOARDWIDTH;
        int y = (i - x) / BOARDWIDTH;

        sf::RectangleShape square(sf::Vector2f(SCALE, SCALE));
        if (board[i] != 0) {
            square.setOutlineThickness(-3);
        } else {
            square.setOutlineThickness(-1);
        }

        square.setOutlineColor(sf::Color(50,50,50));

        square.setPosition(x*SCALE, y*SCALE);

        square.setFillColor(colors[board[i]]);
        window.draw(square);
    }
}

void drawInfo(std::array<int, 4> info) {
    // In order: Level, Lines, Score, Next piece
    int level = info[0];
    int lines = info[1];
    int points = info[2];
    int nextPiece = info[3];

    target_delay = (650 * pow(0.92, level));
    
    txt_level.setString("LEVEL " + std::to_string(level));
    txt_lines.setString(std::to_string(lines) + " LINES");
    txt_score.setString(std::to_string(points) + " POINTS");

    window.draw(txt_level);
    window.draw(txt_lines);
    window.draw(txt_score);

    window.draw(nextSquare);

    for (int i = 0; i < 4; i++) {
        int x = TETROMINOES[info[3]][0][i][0];
        int y = TETROMINOES[info[3]][0][i][1];
        
        NPieceRect.setFillColor(colors[info[3]+1]);
        //NPieceRect.setPosition((BOARDWIDTH*SCALE)+MENUMARGIN+(x*NPieceBlockSize) + (NPIECESIZE/4), MENUMARGIN + (y*NPieceBlockSize)+ (NPIECESIZE/3));
        int xpos = (BOARDWIDTH*SCALE)+MENUMARGIN+(x*NPieceBlockSize);
        int ypos = MENUMARGIN + (y*NPieceBlockSize);
        
        //Correct positioning
        switch(info[3]) {
            case 0: //I
                xpos += NPieceBlockSize;
                ypos += NPieceBlockSize*1.5;
                break;
            case 3: //Sqare / O
                xpos += NPieceBlockSize;
                ypos += NPieceBlockSize*2;
                break;
            default:
                xpos += NPieceBlockSize*1.5;
                ypos += NPieceBlockSize*2;
                break;
        }

        NPieceRect.setPosition(xpos, ypos);
        window.draw(NPieceRect);
    }
}

void initText() {
    font.loadFromFile("Pixelmania.ttf");

    //Initialize text objects for displaying progress
    txt_level.setFont(font);
    txt_level.setCharacterSize(MENUTEXT);
    txt_level.setPosition(MENUMARGIN + (BOARDWIDTH*SCALE), NPIECESIZE + MENUMARGIN*2);
    txt_level.setFillColor(sf::Color::White);

    txt_lines.setFont(font);
    txt_lines.setCharacterSize(MENUTEXT);
    txt_lines.setPosition(MENUMARGIN + (BOARDWIDTH*SCALE), NPIECESIZE + MENUMARGIN*3);
    txt_lines.setFillColor(sf::Color::White);

    txt_score.setFont(font);
    txt_score.setCharacterSize(MENUTEXT);
    txt_score.setPosition(MENUMARGIN + (BOARDWIDTH*SCALE), NPIECESIZE + MENUMARGIN*4);
    txt_score.setFillColor(sf::Color::White);

    //Initialize the everything neccessary to display the next piece
    nextSquare.setSize(sf::Vector2f(NPIECESIZE, NPIECESIZE));
    nextSquare.setPosition((BOARDWIDTH*SCALE)+MENUMARGIN, MENUMARGIN);
    nextSquare.setOutlineThickness(-12);
    nextSquare.setOutlineColor(sf::Color::White);
    nextSquare.setFillColor(sf::Color::Black);

    NPieceRect.setSize(sf::Vector2f(NPieceBlockSize, NPieceBlockSize));
    NPieceRect.setOutlineThickness(-3);
    NPieceRect.setOutlineColor(sf::Color(200,200,200));
    
}

int main(int argc, char *argv[]) {
    Tetris game;
    game.init();
    initText();

    drawBoard(game.getBoard());
    drawInfo(game.getInfo());

    sf::Clock clock;

    int tickCount = 0;

    while (window.isOpen()) {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(100,100,100));

        bool keys[] = {
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
        };

        game.tick(keys, (tickCount == MOVES_PER_TICK));

        if (tickCount == MOVES_PER_TICK) { 
            tickCount = 0;
        } else {
            tickCount += 1;
        }

        //Display game board and stats
        drawBoard(game.getBoard());
        drawInfo(game.getInfo());

        //Delay to maintain correct speed
        sf::Time framePeriod = clock.getElapsedTime();
        while (framePeriod.asMilliseconds() < target_delay / MOVES_PER_TICK) {
            framePeriod = clock.getElapsedTime();
        }
        clock.restart();



        window.display();
    }

    
    return 0;
}