#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>
#include <iostream>
#include "Board.h"
#include "utils.h"

class Player {
    public:
        // constructors
        Player(std::string name);
        // Deconstructor
        ~Player();

        // Returns the player name 
        std::string getName();
        // Returns the player point 
        int getPoint();
        // sets the player points for loading games
        void setPoints(int point);
        // sets the player boards
        void setBoard(Board* board);
        // get the player boards
        Board* getBoard();
		
    private:
        Board* board;
        std::string name;
        int point;
};

#endif // ASSIGN2_PLAYER_H
