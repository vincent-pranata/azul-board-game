#include "Menu.h"

Menu::Menu(){
	game = nullptr;
    seed = 0;
    seedInput = false;
    start();
}

Menu::Menu(int seed){
	game = nullptr;
    this->seed=seed;
    seedInput = true;
    start();
}

Menu::~Menu() {
	delete game;
}

void Menu::start(){
    welcomeMessage();
    menuSelect();
}

void Menu::welcomeMessage(){
    std::cout << "Welcome to Azul!" << std::endl;
    std::cout << "-------------------" << std::endl;
}


void Menu::menuSelect(){
    bool repeat = true;
    std::string userInput = "";
    //while the input is not quit
    while (repeat){
        //prints the options
        std::cout << "\nMenu" << std::endl;
        std::cout << "----" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Credits (Show Student Information)" << std::endl;
        std::cout << "4. Quit\n"
                << std::endl;
        std::cout << "> ";
        std::cin >> userInput;
        eof();

        //switch case for menu options
        if (userInput == "1"){
            newGame();
        }
        else if (userInput == "2"){
            loadGame();
        }
        else if (userInput == "3"){
            showInfo();
        }
        else if (userInput == "4"){
            quit();
        }
        //help function to define what to input for the user
        else if (userInput == "help"){
            std::cout << "\nPlease enter a number between 1- 4 to continue."
                      << std::endl;
        }
        //if there is any other input
        else{
            std::cout << "\nInvalid Input\n"<< std::endl;
        }
    }
}

int Menu::validInput(){
    int x;
    std::cin >> x;
    eof();
    while (std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a numeric value"
                  << std::endl;
        std::cout << ">";
        std::cin >> x;
        eof();
    }
    return x;
}

void Menu::newGame() {
    std::vector<Player*>* players = new std::vector<Player*>();
    int playerCount = 0;
    if(!seedInput){
        srand(time(NULL));
        seed = rand();

    }
    while (playerCount < MIN_PLAYERS || playerCount > MAX_PLAYERS){
        std::cout << "\nEnter number of players (minimum 2, maximum 4)"
                  << std::endl;
        std::cout << ">";
        playerCount = validInput();

        if (playerCount < MIN_PLAYERS || playerCount > MAX_PLAYERS){
            std::cout << "\nError: 2-4 players only";
        }
    }


    std::cout << "Name can only be alphabets. Don't put space since whatever is after the space will be assigned to the next players"<< std::endl;	
    for(int i=0; i<playerCount; i++){
    	std::string name = "";
        bool addPlayer = false;
        while(!addPlayer){
            std::cout << "Enter a Name for Player " << i+1 <<": " << std::endl;	
            std::cout << "> ";
            std::cin >> name;
            eof();
            if (isAlphabet(name)){
                addPlayer = true;
            }
            else{
                std::cout << "Only characters are allowed\n";
            }
        }
        players->push_back(new Player(name));
    }
	game = new GameEngine(players, seed);
}

bool Menu::isAlphabet(std::string player){
    bool result = false;
    for (int i = 0; player[i]!='\0'; i++){
        if (isalpha(player[i])){
            result = true;
        }
        else{
            result = false;
        }
    }
    return result;
}

void Menu::loadGame(){
    std::string fileName = "";
    bool stop = false;
    bool invalidFile = false;
    std::cin.ignore();
    do{
        std::cout << "\nEnter the filename from which to load a game" << std::endl;
        std::cout << "> ";
        std::cin >>fileName;
        eof();
        if (fileName == "\0"){
            stop = true;
        }
        else{
            fileName = "saves/"+fileName + ".save";
            std::ifstream inFile(fileName);
            invalidFile = inFile.good();
            if (!invalidFile){
                std::cout << "\nUnable to open file of " << fileName << std::endl;
            }
            inFile.close();
        }
    } while (!stop && !invalidFile);

    if (!stop){
        game = new GameEngine(fileName);
    }
    else{
        std::cout << "\nGoodbye !" << std::endl;
    }
}

void Menu::showInfo() {
    std::string line;
    std::ifstream rfile;
    rfile.open("showInfo.txt");
    if (rfile.is_open()){
        while (getline(rfile, line)){
            std::cout << line << std::endl;
        }
        rfile.close();
    }
    menuSelect();
}

void Menu::eof() {
    if (std::cin.eof()){
        quit();
    }
}

void Menu::quit() {
    std::cout << "Goodbye" << std::endl;
    exit(EXIT_SUCCESS);
}
