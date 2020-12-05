#include "GameEngine.h"

GameEngine::GameEngine(std::vector<Player*>* players, int seed){
    turn = 0;
    this->players = players;
    this->seed = seed;
    if(players->size()==2){
        numOfFixedFactories=5;
    }
    else if(players->size()==3){
        numOfFixedFactories=7;
    }
    else{
        numOfFixedFactories=9;
    }
    chooseBoard();
    chooseMode();
    numberOfOpenFactories();
    tileBag = new LinkedList();
    tileBag->setSeed(seed);
    lid = new LinkedList();
    lid->setSeed(seed);
    for(unsigned int i=0; i<players->size(); i++){
        Board* board = new Board(boardSize, greyMode);    
        (*players)[i]->setBoard(board);
    }
    createTileBag();
    tileBag->shuffle(); 
    openFactories = new std::vector<std::vector<Tile*>>();
    openFactories->resize(numOfOpenFactories);
    factories = new std::vector<std::vector<Tile*>>();
    factories->resize(numOfFixedFactories);
    for(int i=0; i<numOfFixedFactories; i++)    {
        (*factories)[i] = std::vector<Tile*>();
        (*factories)[i].resize(FACTORY_TILE);
    }
    assignFactories();
    gameLoop(); 
    players->clear();
}

GameEngine::GameEngine(std::string filename) {
    openFactories = new std::vector<std::vector<Tile*>>();
    factories = new std::vector<std::vector<Tile*>>();
    players = new std::vector<Player*>();
    tileBag = new LinkedList();
    lid = new LinkedList();
    if(createSavedGame(filename)){
        std::cin.ignore();
        gameLoop();
        players->clear();
    }
    else{
        std::cout << "Save format corrupted, please check save file" << std::endl;
    }
}

GameEngine::~GameEngine(){
}

bool GameEngine::createSavedGame(std::string filename) {
    std::ifstream infile;
    std::string temp = "";
    bool success = false;
    infile.open(filename);
    while(!infile.eof()){
        std::string line="";
        std::getline(infile, line);
        if(line=="name"){
            std::getline(infile, line);
            players->push_back(new Player(line));
        }
        else if(line=="point"){
            for(unsigned int i=0; i<players->size(); i++ ){
                std::getline(infile, line);
                (*players)[i]->setPoints(std::stoi(line));
            }
        }
        else if(line=="turn"){        
            std::getline(infile, line);
            for(unsigned int i=0; i<players->size(); i++ ){
                if(line==(*players)[i]->getName()){
                    turn=i;
                }
            }
        }
        else if(line=="factory"){
            if(players->size()==2){
                numOfFixedFactories=5;
            }
            else if(players->size()==3){
                numOfFixedFactories=7;
            }
            else{
                numOfFixedFactories=9;
            }
            std::getline(infile, line);
            numOfOpenFactories = std::stoi(line);
            openFactories->resize(numOfOpenFactories);
            for(int i=0; i<numOfOpenFactories; i++){
                (*openFactories)[i] = std::vector<Tile*>();
            }
            factories->resize(numOfFixedFactories);
            for(int i=0; i<numOfFixedFactories; i++){
                (*factories)[i] = std::vector<Tile*>();
                (*factories)[i].resize(FACTORY_TILE);
            }
            
            for (int i=0; i<numOfOpenFactories; i++){    
                std::getline(infile, line);
                if(line.at(0)!='\n'){
                    for(unsigned int j=0;j<line.size();j++) {
                        if(line.at(j)!=' '){
                            (*openFactories)[i].push_back(new Tile(line.at(j)));
                        }
                    }
                }
            }

            for(int i=0;i<numOfFixedFactories;i++) {
                int tileFactoryCounter = 0;
                std::getline(infile, line);
                for(unsigned int j=0;j<line.size();j++) {
                    if(line.at(j)!=' ') {
                        Tile* tile = new Tile(line.at(j));
                        (*factories)[i][tileFactoryCounter] = tile;
                        tileFactoryCounter++;
                    }
                }
            }
        }
        else if(line=="board"){
            std::getline(infile, line);
            boardSize = std::stoi(line);
            std::getline(infile, line);
            if(line=="y"){
                greyMode = true;
            }
            else{
                greyMode = false;
            }
            for(unsigned int k=0; k<players->size(); k++){
                Tile* storage[MAX_SIZE][MAX_SIZE] = {};
                Tile* mosaic[MAX_SIZE][MAX_SIZE] = {};
                Tile* brokenTiles[MAX_NUM_BROKEN_TILES] = {};

                for(int i=0;i<boardSize;i++) {
                    for(int j=0;j<boardSize;j++) {
                        std::getline(infile, line, ' ');
                        Tile* tile = new Tile(line.at(0));
                        mosaic[j][i] = tile;
                    }
                    // to get the '\n' so not assigned 
                    std::getline(infile, line);
                }

                for(int i=0;i<boardSize;i++) {
                    for(int j=0;j<boardSize;j++) {
                        if(i>=j){
                            std::getline(infile, line, ' ');
                            Tile* tile = new Tile(line.at(0));
                            storage[j][i] = tile;
                        }
                    }
                    // to get the '\n' so not assigned 
                    std::getline(infile, line);
                }

                std::getline(infile, line);
                int counter=0;
                for(unsigned int j=0; j<line.size(); j++) {
                    if(line.at(j)!=' ') {
                        Tile* tile = new Tile(line.at(j));
                        brokenTiles[counter] = tile;
                        counter++;
                    }
                }
                Board* board = new Board(storage, mosaic, brokenTiles, boardSize);
                // std::cout << board->printBoard() << std::endl;
                (*players)[k]->setBoard(board);
                // std::cout << (*players)[k]->getBoard()->printBoard() << std::endl;
            }
        }
        else if(line=="lid"){
            // LID
            std::getline(infile, line);
            for(unsigned int i=0;i<line.size();i++) {
                if(line.at(i)!=' ') {
                    Tile* tile = new Tile(line.at(i));
                    lid->addBack(tile);
                }
            }
        }
        else if(line=="bag"){
            // BAG
            std::getline(infile, line);
            for(unsigned int i=0;i<line.size();i++) {
                if(line.at(i)!=' ') {
                    Tile* tile = new Tile(line.at(i));
                    tileBag->addBack(tile);
                }
            }
        }
        else if(line=="seed"){
            std::getline(infile, line);
            //GET SEED
            seed = std::stoi(line);
        }
        success = true;
    }
    infile.close();
    return success;
}

void GameEngine::createTileBag(){
    Colour colourList[TOTAL_COLOUR] = {YELLOW, RED, BLACK, DARKBLUE, LIGHTBLUE, ORANGE};
    for (int i=0; i<TOTAL_TILE_COLOUR; i++){
        // For each Colour
        for (int j=0; j<boardSize; j++){
            // Create a tile
            tileBag->addBack(new Tile(colourList[j]));
        }
    }       
}

void GameEngine::addToLid(Tile *tile) {
    lid->addBack(tile);
}

void GameEngine::refreshTileBag() {
    lid->shuffle();
    for(int i=0;i<lid->size();i++) {
        tileBag->addBack(new Tile(lid->getAndRemoveTile(i)->getColour()));
        i--;
    }
}

void GameEngine::gameLoop(){
    int round=0;
    std::string recap;
    std::cout << "Let's Play!\n"<< std::endl;    
    std::cout << std::endl;
    do{
        std::cout << "=== Start Round ===" << std::endl;
        recap = "";
        while(!checkFactoryIsEmpty()){
            std::string facString = "\n";
            for(unsigned int i=0; i<openFactories->size(); i++){
                facString += RESET_COLOUR;
                facString += "Factory ";
                facString += std::to_string(i);
                facString += ": ";
                for(unsigned int j=0; j<(*openFactories)[i].size(); j++){
                    facString += (*openFactories)[i][j]->getEColour();
                    facString += (*openFactories)[i][j]->getColour();
                    facString += " ";
                }
                facString += "\n";
            }
            for(int i=0;i<numOfFixedFactories;i++) {
                facString += RESET_COLOUR;
                facString += "Factory ";
                facString += std::to_string(i+numOfOpenFactories);
                facString += ": ";

                for(int j=0;j<FACTORY_TILE;j++) {
                    if((*factories)[i][j]!=nullptr){
                        facString += (*factories)[i][j]->getEColour();
                        facString += (*factories)[i][j]->getColour();
                    }
                    else{
                        facString += " ";    
                    }
                    facString += " ";
                }
                facString += "\n";
            }
            std::cout << facString;
            std::cout << RESET_COLOUR;
            std::cout << "\nMosaic for " << (*players)[turn%players->size()]->getName() << ":" << std::endl;
            std::cout << (*players)[turn%players->size()]->getBoard()->printBoard();

            std::cout << "Input 'help' to print out some commands you can do" << std::endl;
            
            std::string answer="";
            bool validInput = false;
            while(!validInput){
                answer = getPlayerAction();
                validInput=valdidateAction(answer);
                if(validInput){
                    recap += (*players)[turn%players->size()]->getName() + ": " + answer + "\n";
                    std::cout << std::endl;
                }
            }
            turn++;
        }

        std::cout << lid->printData() << std::endl;
        for(unsigned int k=0; k<players->size(); k++){
            //assign full storage to mosaic 
            if(!greyMode){
                (*players)[k%players->size()]->getBoard()->assignToMosaic();
            }
            else{
                std::cout << (*players)[k%players->size()]->getName() << "'s Tile Placement to Mosaic" << std::endl << std::endl;
                std::cout << (*players)[k%players->size()]->getBoard()->printBoard() << std::endl;
                for(int i=1; i<=boardSize; i++){
                    if((*players)[k%players->size()]->getBoard()->checkRowIfFull(i)){
                        bool valid=false;
                        int num = 0;
                        while(!valid){
                            while(num<1 || num>boardSize){
                                std::cout << "Choose a column of your mosaic row " << i << " (1 to " << boardSize << ")" << std::endl << "> ";
                                num = validNum();
                                if(num<1 || num>boardSize){
                                    std::cout << "Error: Invalid column. You can only choose from 1 to " << boardSize << std::endl;
                                }
                                else{
                                    std::cin.ignore();
                                }
                            }
                            if(((*players)[k%players->size()]->getBoard()->checkAdvancedMosaic(i, num))){
                                valid=true;
                            }
                            else{
                                num = 0;
                                std::cout << "Error: Invalid mosaic, the tile already exist in col " << num << " or its not a . tile" << std::endl;
                            }
                        }
                        (*players)[k%players->size()]->getBoard()->assignToMosaicGrey(i, num);                            
                    }
                }
            }
            //remove tile from board and add it to lid
            for(int i=1; i<=boardSize; i++){
                Tile* tile = (*players)[k%players->size()]->getBoard()->getTileIfRowFull(i);
                if(tile!=nullptr){
                    for(int j=0; j<i-1; j++){
                        addToLid(tile);
                    }
                }
            }
            
            updatePoint(k);
            turn = setFirstTurn();
            // remove broken tiles to lid 
            for(int i=0; i<brokenTileSize; i++){
                Tile* tile = (*players)[k%players->size()]->getBoard()->getBrokenTile(i);
                if(tile!=nullptr){
                    if(tile->getColour()!=FIRSTPLAYER){
                        addToLid(tile);
                    }
                }
            }
            
        }
        std::cout << "=== End Round ===" << std::endl;
        for(unsigned int k=0; k<players->size(); k++){
            std::cout << (*players)[k]->getName() << "'s Points: "<< (*players)[k]->getPoint() << std::endl;
        }
        assignFactories();
        if(tileBag->size()==0){
            refreshTileBag();
        }
        round++;
    }while(!gameEnd());
    int winner = 0;  
    for(unsigned int i=0;i<players->size();i++) {
        if((*players)[i]->getPoint()>(*players)[winner]->getPoint()) {
            winner = i;
        }
    }
    std::cout << "Congratulations " << (*players)[winner]->getName() << "!" << std::endl;
    std::cout << "You are the winner!" << std::endl;
}   

bool GameEngine::valdidateAction(std::string input){
    bool valid = false;
    if(input.size()>=4){
        if(input.substr(0,4) == "save"){
            if(input.size()>=6){
                std::string filename = input.substr(5, input.size()-5);
                saveGame(filename);
                std::cout << "Game is saved as " << filename << std::endl;                  
            }
            else{
                std::cout << "Please also input the filename.\nFor example: save VincentAndLouis" << std::endl;
            }
        }
        else if(input.substr(0,4) == "turn"){
            if(input.size()==10 || input.size()==11){
                std::string factoryNumber = "";
                std::string tile = "";
                std::string storageRow = "";
                if(input.size()==11){
                    factoryNumber = input.substr(5,2);
                    tile = input.substr(8,1);
                    storageRow = input.substr(10,1);
                }
                else if(input.size()==10){
                    factoryNumber = input.substr(5,1);
                    tile = input.substr(7,1);
                    storageRow = input.substr(9,1);
                }

                
                int totalTile = 0;
                //change factory number and storage row to equal number  
                if(isdigit(factoryNumber.at(0)) && isalpha(tile.at(0)) && isdigit(storageRow.at(0))){
                    if(valdidateMove((*players)[turn%players->size()]->getBoard(), std::stoi(factoryNumber), std::stoi(storageRow), toupper(tile.at(0)))){
                        valid = true;
                        if(std::stoi(factoryNumber)>=numOfOpenFactories){
                            int num=-1;
                            while (num<0 || num>=numOfOpenFactories){
                                if(numOfOpenFactories==1){
                                    num = 0;
                                }
                                if(numOfOpenFactories==2){
                                    std::cout << "\nChoose open factory (0 or 1)" << std::endl;
                                    std::cout << "> ";
                                    num = validNum();
                                    std::cin.ignore();
                                }
                                if (num<0 || num>=numOfOpenFactories){
                                    std::cout << "\nError: You can only choose 0 or 1";
                                }
                                else{
                                    for(int j=0; j<FACTORY_TILE; j++){
                                        if((*factories)[std::stoi(factoryNumber)-numOfOpenFactories][j]->getColour()==toupper(tile.at(0))){
                                            (*factories)[std::stoi(factoryNumber)-numOfOpenFactories][j] = nullptr;
                                            totalTile++;
                                        }
                                        else{
                                            char temp = (*factories)[std::stoi(factoryNumber)-numOfOpenFactories][j]->getColour();
                                            if(numOfOpenFactories==2 ){
                                                (*openFactories)[num].push_back(new Tile(temp));    
                                            }
                                            else{
                                                (*openFactories)[0].push_back(new Tile(temp));
                                            }
                                            (*factories)[std::stoi(factoryNumber)-numOfOpenFactories][j] = nullptr;  
                                        }
                                    }
                                }
                            }
                        }
                        else if(std::stoi(factoryNumber)>=0 || std::stoi(factoryNumber)<numOfOpenFactories){
                            for(unsigned int j=0; j<(*openFactories)[std::stoi(factoryNumber)].size(); j++){
                                if((*openFactories)[std::stoi(factoryNumber)].size()>0){ 
                                    if((*openFactories)[std::stoi(factoryNumber)][j]->getColour()==toupper(tile.at(0))){
                                        (*openFactories)[std::stoi(factoryNumber)].erase((*openFactories)[std::stoi(factoryNumber)].begin()+j);
                                        totalTile++;
                                        //because the next tile is moved to the slot we just check n it wont be checked                            
                                        j--;
                                    }
                                }
                            }
                            if((*openFactories)[0].size()>0){    
                                //removes F if exist in factory 0 and set it to player's broken tile
                                bool remove = false;
                                for(unsigned int i=0; i<openFactories->size(); i++){
                                    if(totalTile>0 && (*openFactories)[i][0]->getColour()==FIRSTPLAYER){
                                        (*openFactories)[i].erase((*openFactories)[i].begin());
                                        remove =true;
                                    }
                                }
                                if(remove){
                                    (*players)[turn%players->size()]->getBoard()->setBrokenTiles(1, new Tile(FIRSTPLAYER));
                                }
                            }
                        }
                        totalTile = (*players)[turn%players->size()]->getBoard()->setTileStorage(std::stoi(storageRow)-1, totalTile, new Tile(toupper(tile.at(0))));
                        if(totalTile>0){
                            while(totalTile>0){
                                addToLid(new Tile(toupper(tile.at(0))));
                                totalTile--;
                            }
                        }
                    }
                    else{
                        std::cout << "Invalid move" << std::endl;        
                    }
                }
                else{
                    std::cout << "Tiles can only be alphabets while factory row and storage row are numbers" << std::endl;
                }
            }
            else{
                std::cout << "Wrong format of the command.\nFor example: turn <factory number> <tile colour> <storage row>" << std::endl;
            }
        }
        else if(input == "help"){
            std::cout << "You can choose to save, do your turn, or print player board" << std::endl;
            std::cout << "Print all enemy boards by input 'board'" << std::endl;
            std::cout << "Save a game by inputting 'save <filename>'" << std::endl;
            std::cout << "Do your turn by inputting 'turn <factory number> <tile> <storage row>'" << std::endl;
        }
        else if(input == "board"){
            for (unsigned i=0; i<players->size(); i++){
                if(i!=(turn%players->size())){
                    std::cout << "Mosaic for " << (*players)[i]->getName() << ":" << std::endl;
                    std::cout << (*players)[i]->getBoard()->printBoard();
                }
            } 
        }
        else{
            std::cout << "Invalid command. Please try again" << std::endl;
        }
    }
    else{
        std::cout << "Invalid command. Please try again" << std::endl;
    }
    return valid;
}   

int GameEngine::validNum(){
    int x;
    std::cin >> x;
    eof();
    while (std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a numeric value"
                  << std::endl;
        std::cout << "> ";
        std::cin >> x;
        eof();
    }
    return x;
}


bool GameEngine::valdidateMove(Board* board, int factoryNumber, int storageRow, Colour colour){
    bool valid = false;
    if(factoryNumber<numOfOpenFactories && factoryNumber>=0){
        if((*openFactories)[factoryNumber].size()>0){
            if(colour==YELLOW || colour==RED || colour==DARKBLUE || colour==LIGHTBLUE || colour==BLACK || (boardSize==6 && colour==ORANGE)){
                for(unsigned int j=0; j<(*openFactories)[factoryNumber].size(); j++){
                    if((*openFactories)[factoryNumber][j]->getColour()==colour){
                        valid = true;
                    }
                }
                if(!valid){
                    std::cout << "Tile doesn't exist in the factory, please try again" << std::endl;
                }         
                else{
                    if(storageRow>=1 && storageRow<=boardSize){
                        valid = board->checkStorage(storageRow, colour);
                        if(!valid){
                            std::cout << "Another colour is already in the row, please choose another row" << std::endl;
                        }
                        else{
                            valid = !(board->checkRowIfFull(storageRow));
                            if(!valid){
                                std::cout << "Row is full" << std::endl;
                            }                        
                            else{
                                valid = board->checkMosaic(storageRow, colour);
                                if(!valid){
                                    std::cout << "Tile is already placed in mosaic, please choose another tile/row" << std::endl;    
                                }
                            }
                        }
                    }
                    else if(storageRow==boardSize+1){
                        valid = true;
                    }
                }
            }
            else if(colour==FIRSTPLAYER){
                if(boardSize==6){
                    std::cout << "Can't pick tile F, please choose between R,L,B,U,Y, or O" << std::endl;
                }
                else{
                    std::cout << "Can't pick tile F, please choose between R,L,B,U, or Y" << std::endl;                   

                }
            }
            else{
                if(boardSize==6){
                    std::cout << "Invalid tile, please choose between R,L,B,U,Y, or O" << std::endl;
                }
                else{
                    std::cout << "Invalid tile, please choose between R,L,B,U or Y" << std::endl;
                }
            }
        }
        else{
            std::cout << "Factory is empty, please pick another factory" << std::endl;
        }
    
    }
    else if(storageRow==0){
        std::cout << "Invalid storage Row, cannot have 0 as storage row." << std::endl;
    }
    else if(factoryNumber>0 && factoryNumber<numOfFixedFactories+numOfOpenFactories){
        if((*factories)[factoryNumber-numOfOpenFactories][0]!=nullptr){
            if(colour==YELLOW || colour==RED || colour==DARKBLUE || colour==LIGHTBLUE || colour==BLACK || (boardSize==6 && colour==ORANGE)){
                for(int j=0; j<FACTORY_TILE; j++){
                    if((*factories)[factoryNumber-numOfOpenFactories][j]->getColour()==colour){
                        valid = true;
                    }
                }     
                if(!valid){
                    std::cout << "Tile doesn't exist in the factories, please try again" << std::endl;
                }   
                else{
                    if(storageRow>=1 && storageRow<=boardSize){
                        valid = board->checkStorage(storageRow, colour);
                        if(!valid){
                            std::cout << "Another colour is already in the row, please choose another row" << std::endl;
                        }
                        else{
                            valid = !(board->checkRowIfFull(storageRow));
                            if(!valid){
                                std::cout << "Row is full" << std::endl;
                            }  
                            else{
                                valid = board->checkMosaic(storageRow, colour);
                                if(!valid){
                                    std::cout << "Tile is already placed in mosaic, please choose another tile/row" << std::endl;    
                                }
                            }                      
                        }
                    }
                    else if(storageRow==boardSize+1){
                        valid = true;
                    }
                }
            }
            else{
                if(boardSize==6){
                    std::cout << "Invalid tile, please choose between R,L,B,U,Y, or O" << std::endl;
                }
                else{
                    std::cout << "Invalid tile, please choose between R,L,B,U or Y" << std::endl;
                }
            }
        }
        else{
            std::cout << "Factory is empty, please pick another factory" << std::endl;
        }
    }
    else{
        std::cout << "Invalid factory number, please pick a factory number from 0-" << numOfFixedFactories+numOfOpenFactories-1 << std::endl;
    }
    return valid;
}

std::string GameEngine::getPlayerAction(){
    std::string result = "";
    std::cout << (*players)[turn%players->size()]->getName() << "'s Turn. What do you want to do?" << std::endl << "> ";
    std::getline(std::cin, result);
	eof();
    return result;
}

bool GameEngine::checkFactoryIsEmpty(){
    bool empty = true;


    for(unsigned int i=0; i<openFactories->size(); i++){
        if((*openFactories)[i].size()>0){
            empty = false;
        }
    }

    for(int i=0; i<numOfFixedFactories; i++){
        for(int j=0; j<FACTORY_TILE; j++){
            if((*factories)[i][j]!=nullptr){
                empty=false;
            }
        }
    }

    return empty;
}

void GameEngine::assignFactories(){
    for(unsigned int i=0; i<openFactories->size(); i++){
        (*openFactories)[i].push_back(new Tile(FIRSTPLAYER));
    }
    for (int i=0; i < numOfFixedFactories; i++){
        for (int j = 0; j < FACTORY_TILE; j++){
            if(tileBag->size()>0){
                // Add tiles from bag to factories
                (*factories)[i][j]=tileBag->getAndRemoveTile(0);
            }
        }
    }
}


void GameEngine::saveGame(std::string filename){
	std::string name=filename;
    std::ofstream file;
    std::string factoryString = "";
    std::string mosaics = "";
    std::string brokenTilesString = "";
    std::string lidTiles = "";
    std::string bagTiles = "";

    for(unsigned int i=0; i<openFactories->size(); i++){
        for(unsigned int j=0; j<(*openFactories)[i].size(); j++){
            factoryString+=(*openFactories)[i][j]->getColour();
            factoryString+=" ";
        }
        factoryString+="\n";
    }

    for(int i=0;i<numOfFixedFactories;i++) {
        for(int j=0;j<FACTORY_TILE;j++) {
            if((*factories)[i][j]!=nullptr) {
                factoryString += (*factories)[i][j]->getColour();
            }
            else{
                factoryString += " ";                    
            }
            factoryString += " ";
        }
        factoryString += "\n";
    }

    for(int i=0;i<lid->size();i++){
        lidTiles += lid->getTile(i)->getColour();
        lidTiles += " ";
    }

    for(int i=0;i<tileBag->size();i++) {
        bagTiles += tileBag->getTile(i)->getColour();
        bagTiles += " ";
    }

    name+=".save";
    name= "saves/" +name;
    file.open(name);
    for(unsigned int i=0; i<players->size(); i++) {
        file << "name\n" << (*players)[i]->getName() << "\n";
    }
    file << "point\n";
    for(unsigned int i=0; i<players->size(); i++) {
        file << (*players)[i]->getPoint() << "\n";
    }
    file << "turn\n" << (*players)[turn%players->size()]->getName() << "\n";
    file << "factory\n" << numOfOpenFactories << "\n" << factoryString;
    file << "board\n";
    file << boardSize << "\n";
    if(greyMode){
        file << "y" << "\n";
    }
    else{
        file << "n" << "\n";    
    }
    for(unsigned int i=0; i<players->size(); i++) {
        file << (*players)[i]->getBoard()->getMosaic();
        file << (*players)[i]->getBoard()->getStorage();
        file << (*players)[i]->getBoard()->printBrokenTiles() << "\n";
    }
    file << "lid\n" << lidTiles << "\n" << "bag\n" << bagTiles << "\n" << "seed\n" << seed;
    file.close();
}

void GameEngine::quit(){
    std::cout << "\nQuitting ..." << std::endl;
    std::cout << "File will not be save\n" << std::endl;
    std::cout << "Goodbye" << std::endl;
    exit(EXIT_SUCCESS);
}

void GameEngine::eof() {
    if (std::cin.eof()){
        quit();
    }
}

void GameEngine::updatePoint(unsigned int i) {
    (*players)[i]->setPoints((*players)[i]->getPoint() + (*players)[i]->getBoard()->calculatePoints());
}

int GameEngine::setFirstTurn() {
    int firstPlayer = 0;
    for(unsigned int i=0; i<players->size(); i++) {
        if((*players)[i]->getBoard()->checkFirstTurn()) {
            firstPlayer = i;
        }
    }
    return firstPlayer;
}

bool GameEngine::gameEnd() {
    bool end = false;
    for(unsigned int i=0; i<players->size(); i++) {
        if((*players)[i]->getBoard()->checkGameEnd() || (*players)[i]->getBoard()->checkGameEnd()) {
            end = true;
        }
    }
    return end;
}

void GameEngine::numberOfOpenFactories() {
    numOfOpenFactories=0;
    while (numOfOpenFactories<1 || numOfOpenFactories>2){
        std::cout << "\nEnter number of open factory (1 or 2)"
            << std::endl;
        std::cout << "> ";
        numOfOpenFactories = validNum();

        if (numOfOpenFactories<1 || numOfOpenFactories>2){
            std::cout << "\nError: You can only have 1 or 2 open factories";
        }
        else{
            std::cin.ignore();
        }
    }
}

void GameEngine::chooseMode() {
    int num = 0;
    std::cout << "\n1. Normal Mode";
    std::cout << "\n2. Grey Tile Mode" << std::endl;

    while (num<1 || num>2){
        std::cout << "\nChoose a game mode (1 or 2)"
            << std::endl;
        std::cout << "> ";
        num = validNum();

        if (num<1 || num>2){
            std::cout << "\nError: You can only choose either 1 or 2";
        }
        else{
            std::cin.ignore();
            if(num==1){
                greyMode = false;
            }
            else{
                greyMode = true;
            }
        }
    }
}

void GameEngine::chooseBoard() {
    boardSize=0;
    while (boardSize<5 || boardSize>6){
        std::cout << "\nEnter the board size (5 or 6)"
            << std::endl;
        std::cout << "> ";
        boardSize = validNum();

        if (boardSize<5 || boardSize>6){
            std::cout << "\nError: You can only have 5x5 or 6x6 sized board";
        }
        else{
            std::cin.ignore();
            if(boardSize==MAX_SIZE){
                brokenTileSize = MAX_NUM_BROKEN_TILES;
            }
            else{
                brokenTileSize = MAX_NUM_BROKEN_TILES-1;
            }
        }
    }
}
