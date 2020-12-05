#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include <string>

#include "Tile.h"

const int MAX_SIZE = 6;
const std::string rows[MAX_SIZE] = {"1","2","3","4","5","6"};
const char pattern[MAX_SIZE] = {'b','y','r','u','l','o'};

class Board {
    public:
        // constructors
        Board(int boardSize, bool greyMode);
        Board(Tile* storage[MAX_SIZE][MAX_SIZE], Tile* mosaic[MAX_SIZE][MAX_SIZE], Tile* getBrokenTiles[MAX_NUM_BROKEN_TILES], int boardSize);  
        // Deconstructor
        ~Board();
        // sets tile in specified storage row
        int setTileStorage(int row, int totalTiles ,Tile* tile);
        // sets tile in specified storage row
        int setBrokenTiles(int totalTiles ,Tile* tile);
        // set tile from row to board for normal mode
        void assignToMosaic();
        // set tile from row to board for grey mode
        void assignToMosaicGrey(int row, int col);
        // Returns the Board's string appearance
        std::string printBoard();
        // Returns Board's mosaic
        std::string getMosaic();
        // Returns Board's storage
        std::string getStorage(); 
        // Returns Board's broken tiles
        std::string printBrokenTiles();
        //return broken tiles
        Tile* getBrokenTile(int position);
        // check if row is already full
        bool checkRowIfFull(int row);
        // check if picked tile is the same colour with tile in row
        bool checkStorage(int row, Colour tile);
        // check if picked tile is already filled in mosaic of the row
        bool checkMosaic(int row, Colour tile);
        // check if picked tile is already filled in mosaic row and col
        bool checkAdvancedMosaic(int row, int col);
        // check if row full 
        Tile* getTileIfRowFull(int row);
        // Calculate end of round points for a board
        int calculatePoints();
        // Calculate subtracted points from broken tiles
        int subtractBrokenPoints();
		// checks broken tiles for the first turn tile
        bool checkFirstTurn();
        // check if theres a full row
        bool checkGameEnd();
        // check if real tile
        bool checkLowerCase(Tile* tile);

    private:
        Tile* storage[MAX_SIZE][MAX_SIZE];
        Tile* mosaic[MAX_SIZE][MAX_SIZE];
		Tile* brokenTiles[MAX_NUM_BROKEN_TILES] = {};
        int boardSize=0;
        int brokenTileSize=0;
};

#endif // ASSIGN2_BOARD_H
