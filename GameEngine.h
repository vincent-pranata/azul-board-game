#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <exception>
#include <climits>


#include "Board.h"
#include "Player.h"
#include "LinkedList.h"
#include "Tile.h"
#include "utils.h"

class GameEngine{
	public:
		GameEngine(std::vector<Player*>* players, int seed);
		GameEngine(std::string filename);
		~GameEngine();
		// main game loop
		void gameLoop();
		//when loading in a game file
		bool createSavedGame(std::string filename);
		//create a save file
		void saveGame(std::string filename);
		//create a tile bag with 20 of each tile
		void createTileBag();
		//add tiles from storage, brokentiles to lid
		void addToLid(Tile *tile);
		//put tiles from lid into bag
		void refreshTileBag();
		//check if any players mosaic is full
		bool checkEndGame();
		//check factory is empty for round
		bool checkFactoryIsEmpty();
		//print a message when quitting 
		void quit();
		//check if inputting eof
		void eof();
		//get player input
		std::string getPlayerAction();
		//check if input is valid number
		int validNum();
		//validate if player command is valid
		bool valdidateAction(std::string input);
		//after round end assign tiles from bag to factories
		void assignFactories();
		//validate if player command for turn is valid
		bool valdidateMove(Board* board, int factoryNumber, int storageRow, Colour colour);
		//update player points
		void updatePoint(unsigned int i);
		//set next first turn to player who have first player tile
		int setFirstTurn();
		//end the game
		bool gameEnd();
		//choose mode (greytile or normal)
		void chooseMode();
		//choose number of open factory 
		void numberOfOpenFactories();
		//choose board size
		void chooseBoard();
		
	private:
	    std::vector<Player*>* players = nullptr;
		LinkedList* tileBag = nullptr; 
		LinkedList* lid = nullptr;
		std::vector<std::vector<Tile*>>* factories = nullptr;
		std::vector<std::vector<Tile*>>* openFactories = nullptr;
		int turn = 0;
		int numOfOpenFactories = 0;
		int numOfFixedFactories = 0;
		int boardSize = 0;
		bool greyMode = false;
		int brokenTileSize = 0;
		int seed = 0;
};
#endif // ASSIGN2_GAMEENGINE_H
