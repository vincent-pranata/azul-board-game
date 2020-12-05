#ifndef ASSIGN2_MENU_H
#define ASSIGN2_MENU_H

#include "GameEngine.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <limits>

#include "GameEngine.h"
#include "Player.h"
#include "utils.h"

class Menu
{
	public:
		// Constructors
		Menu();
		// Constructors with seed
		Menu(int seed);
		// Deconstructors
		~Menu();

		// Runs Starting configuration
		void start();
		// Displays a welcoming message
		void welcomeMessage();
		// Displays a choice of options at starting menu
		void menuDialog();
		// Reads Input for starting options
		void menuSelect();
		// Starts a new game
		void newGame();
		// loads a game
		void loadGame();
		// Shows the team that worked on this project
		void showInfo();
		//check if input is valid number of player
		int validInput();
		//check if name input is valid
		bool isAlphabet(std::string player);
		//checks for EOF character
		void eof();
		// Quits Azul
		void quit();
		
	private:
		GameEngine* game;
		int seed;
		bool seedInput;
};

#endif // ASSIGN2_MENU_H
