#include "Board.h"

#include <iostream>

Board::Board(int boardSize, bool greyMode){
    this->boardSize=boardSize;
    if(boardSize==MAX_SIZE){
        brokenTileSize = MAX_NUM_BROKEN_TILES;  
    }
    else{
        brokenTileSize = MAX_NUM_BROKEN_TILES-1;
    }
    for(int i=0; i<boardSize; i++){
        for(int j=0; j<boardSize; j++){
            if(i>=j){
                storage[j][i] = new Tile();
            }
            else{
                storage[j][i] = nullptr;
            }
        }
    } 

    if(!greyMode){
        int row2Counter=boardSize-1;
        int row3Counter=boardSize-2;
        int row4Counter=boardSize-3;
        int row5Counter=boardSize-4;
        int row6Counter=boardSize-5;
        for(int i=0; i<boardSize; i++){
            for(int j=0; j<boardSize; j++){
                if(i==0){
                    mosaic[j][i] = new Tile(pattern[j]);
                }
                else if(i==1){
                    mosaic[j][i] = new Tile(pattern[row2Counter]);
                    row2Counter++;
                    if(row2Counter==boardSize){
                        row2Counter=0;
                    }
                }
                else if(i==2){
                    mosaic[j][i] = new Tile(pattern[row3Counter]);
                    row3Counter++;
                    if(row3Counter==boardSize){
                        row3Counter=0;
                    }
                }
                else if(i==3){
                    mosaic[j][i] = new Tile(pattern[row4Counter]);
                    row4Counter++;
                    if(row4Counter==boardSize){
                        row4Counter=0;
                    }
                }
                else if(i==4){
                    mosaic[j][i] = new Tile(pattern[row5Counter]);
                    row5Counter++;
                    if(row5Counter==boardSize){
                        row5Counter=0;
                    }
                }
                else if(i==5){
                    mosaic[j][i] = new Tile(pattern[row6Counter]);
                    row6Counter++;
                    if(row6Counter==boardSize){
                        row6Counter=0;
                    }
                }
            }
        }
    }
    else{
        for(int i=0; i<boardSize; i++){
            for(int j=0; j<boardSize; j++){
                mosaic[j][i] = new Tile(NOTILE);            
            }
        }
    }

    for(int i=0; i<brokenTileSize; i++){
        brokenTiles[i] = nullptr;
    }
}


Board::Board ( Tile* storage[MAX_SIZE][MAX_SIZE], Tile* mosaic[MAX_SIZE][MAX_SIZE], Tile* brokenTiles[MAX_NUM_BROKEN_TILES], int boardSize){
    this->boardSize=boardSize; 
    for(int i=0; i<boardSize; i++){
        for(int j=0; j<boardSize; j++){
            this->storage[j][i] =storage[j][i];
            this->mosaic[j][i] = mosaic[j][i];
        }
    } 
    for(int i=0; i<brokenTileSize; i++){
        this->brokenTiles[i] = brokenTiles[i];
    }
}

Board::~Board(){
}

int Board::setTileStorage(int row, int totalTiles ,Tile* tile){
    int total = totalTiles;
    if(row>=0 && row<5){
        for(int j=0; j<boardSize; j++){
            if(storage[j][row]!=nullptr && total>0){
                if(storage[j][row]->getColour()==NOTILE){
                    storage[j][row] = tile;
                    total--;
                }
            }
        }
    }
    if(total>0){
        total = setBrokenTiles(total, tile);
    }
    return total;
}

int Board::setBrokenTiles(int totalTiles ,Tile* tile){
    int total = totalTiles;
    for(int i=0; i<brokenTileSize; i++){
        if(brokenTiles[i]==nullptr && total>0){
            brokenTiles[i] = tile;
            total--;
        }
    }
    return total;
}

std::string Board::printBoard(){
    std::string body ="";
    //creating the body of the board
    
    for (int i=0; i<boardSize; i++){
        body += RESET_COLOUR;
        body += rows[i];
        body += ": ";
        for (int j=boardSize-1; j>=0; j--){
            if(storage[j][i]!=nullptr){
                body += storage[j][i]->getEColour();
                body += storage[j][i]->getColour();
                body += " ";
            }
            else{
                body += "  ";
            }
        }
        body += RESET_COLOUR;
        body += " || ";
        for (int j=0; j<boardSize; j++){
            if(mosaic[j][i]!=nullptr){
                body += mosaic[j][i]->getEColour();
                body += mosaic[j][i]->getColour();
                body += " ";
            }
        }
        body += "\n";
    }
    body += RESET_COLOUR;
    body += "broken: ";
    for (int i=0; i<brokenTileSize; i++){
        if(brokenTiles[i]!=nullptr){
            body += brokenTiles[i]->getEColour();
            body += brokenTiles[i]->getColour();
        }
        else{
            body += " ";
        }
        body += " ";
    }
    body += "\n";
    body += RESET_COLOUR;
    return body;
}

std::string Board::getMosaic(){
    std::string print = "";
    for (int i=0; i<boardSize; i++){
        for (int j=0; j<boardSize; j++){
            if(mosaic[j][i]!=nullptr){
                print += mosaic[j][i]->getColour();
                print += " ";
            }
        }
        print+="\n";
    }
    return print;
}

std::string Board::getStorage() {
    std::string print = "";
    for (int i=0; i<boardSize; i++){
        for (int j=0; j<boardSize; j++){
            if(storage[j][i]!=nullptr){
                print += storage[j][i]->getColour();
                print += " ";
            }
        }
        print+="\n";
    }
    return print;
}

std::string Board::printBrokenTiles(){
    std::string print = "";
    for (int i=0; i<brokenTileSize; i++){
        if(brokenTiles[i]!=nullptr){
            print += brokenTiles[i]->getColour();
        }
        else{
            print += " ";
        }
        print += " ";
    }
    return print;
}

Tile* Board::getBrokenTile(int position){
    Tile* tile = nullptr;
    if(brokenTiles[position]!=nullptr){
        tile = new Tile(brokenTiles[position]->getColour());
        brokenTiles[position] = nullptr;
    }
    return tile;
}

void Board::assignToMosaic(){
    int row1Counter = 0;
    int row2Counter = 0;
    int row3Counter = 0;
    int row4Counter = 0;
    int row5Counter = 0;
    int row6Counter = 0;
    
    for (int i=0; i<boardSize; i++){
        for (int j=0; j<boardSize; j++){
            if(storage[j][i]!=nullptr){
                if(storage[j][i]->getColour()!=NOTILE){
                    if(i==0){
                        row1Counter++;
                    }
                    else if(i==1){
                        row2Counter++;
                    } 
                    else if(i==2){
                        row3Counter++;
                    }
                    else if(i==3){
                        row4Counter++;
                    }
                    else if(i==4){
                        row5Counter++;
                    }
                    else if(i==5){
                        row6Counter++;
                    }
                }
            }
        }
    }
    
    for(int i=0; i<boardSize; i++){
        if(row1Counter==1){
            if(toupper(mosaic[i][0]->getColour())==storage[0][0]->getColour()){
                mosaic[i][0] = new Tile(storage[0][0]->getColour());
            }
        }
        if(row2Counter==2){
            if(toupper(mosaic[i][1]->getColour())==storage[0][1]->getColour()){
                mosaic[i][1] = new Tile(storage[0][1]->getColour());
            }
        } 
        if(row3Counter==3){
            if(toupper(mosaic[i][2]->getColour())==storage[0][2]->getColour()){
                mosaic[i][2] = new Tile(storage[0][2]->getColour());
            }
        }
        if(row4Counter==4){
            if(toupper(mosaic[i][3]->getColour())==storage[0][3]->getColour()){
                mosaic[i][3] = new Tile(storage[0][3]->getColour());
            }
        }
        if(row5Counter==5){
            if(toupper(mosaic[i][4]->getColour())==storage[0][4]->getColour()){
                mosaic[i][4] = new Tile(storage[0][4]->getColour());
            }
        }
        if(row6Counter==6){
            if(toupper(mosaic[i][5]->getColour())==storage[0][5]->getColour()){
                mosaic[i][5] = new Tile(storage[0][5]->getColour());
            }
        }
    }
}

bool Board::checkStorage(int row, Colour tile){
    bool valid = false;
    if(storage[0][row-1]->getColour()==tile || storage[0][row-1]->getColour()==NOTILE){
        valid = true;
    }
    return valid;
}

bool Board::checkMosaic(int row, Colour tile){
    bool valid = true;
    for(int i=0; i<boardSize; i++){
        if(mosaic[i][row-1]->getColour()==tile){
            valid = false;
        }
    }
    return valid;
}

bool Board::checkAdvancedMosaic(int row, int col){
    bool valid = true;
    //check row
    for(int i=0; i<boardSize; i++){
        if(mosaic[col-1][row-1]->getColour()!=NOTILE){
            valid = false;
        }
    }
    //check col
    for(int i=0; i<boardSize; i++){
        if(mosaic[col-1][i]->getColour()==storage[0][row-1]->getColour()){
            valid = false;
        }
    }
    return valid;
}

void Board::assignToMosaicGrey(int row, int col){
    mosaic[col-1][row-1] = new Tile(storage[0][row-1]->getColour());
}

bool Board::checkRowIfFull(int row){
    bool full = false;
    int totalTile=0;
    for(int i=0; i<row; i++){
        if(storage[i][row-1]->getColour()!=NOTILE){
            totalTile++;
        }
    }
    if(totalTile==row){
        full = true;
    }
    return full;
}       


//call the calculate point method in this method
Tile* Board::getTileIfRowFull(int row){
    Tile* tile = nullptr;
    int totalRowTile = 0;
    for(int i=0; i<boardSize; i++){
        if(storage[i][row-1]!=nullptr){
            if(storage[i][row-1]->getColour()!=NOTILE){
                totalRowTile++;
            }
        }
    }
    if(totalRowTile==row){
        tile = new Tile(storage[0][row-1]->getColour());
        for(int i=0; i<boardSize; i++){
            if(storage[i][row-1]!=nullptr){
                storage[i][row-1] = new Tile();
            }
        }
    }
    return tile;
}

int Board::calculatePoints() {
    // Rather than loop through multiple times to add horizontal and vertical rows etc, 
    // tile can be set to either a nomal tile(worth one point) or a junction tile (worth 2 points)
    int points = 0;
    int normalTile = 0;
    int junctionTile = 0;
    bool hasHorizontal;
    bool hasVertical;

    for(int i=0;i<boardSize;i++) {
        for(int j=0;j<boardSize;j++) {
            hasHorizontal = false;
            hasVertical = false;
            if(checkLowerCase(mosaic[i][j])) {
                if(checkLowerCase(mosaic[i+1][j]) || checkLowerCase(mosaic[i-1][j])) {
                hasVertical = true;
                }
                if(checkLowerCase(mosaic[i][j+1]) || checkLowerCase(mosaic[i][j-1])) {
                hasHorizontal = true;
                }
                if(hasVertical && hasHorizontal) {
                junctionTile++;
                }
                else {
                    normalTile++;
                }
            }
        }
    }
    points = normalTile + (junctionTile*2) - subtractBrokenPoints();
    return points;
}

int Board::subtractBrokenPoints() {
    int brokenPoints = 0;
    for(int i=0;i<brokenTileSize;i++) {
        if(brokenTiles[i]!=nullptr) {
            if(i==1 || i==2) {
            brokenPoints++;
            }
            else if(i==3 || i==4 || i==5) {
                brokenPoints = brokenPoints+2;
            }
            else if(i==6 || i==7) {
                brokenPoints = brokenPoints+3;
            }
            else if(i==8){
                brokenPoints = brokenPoints+5;
            }
        }
    }
    return brokenPoints;
}

bool Board::checkFirstTurn() {
    bool firstTurn = false;
    for(int i=0;i<brokenTileSize;i++) {
        if(brokenTiles[i]!=nullptr){
            if(brokenTiles[i]->getColour()==FIRSTPLAYER) {
                firstTurn = true;
            }
        }
    }
    return firstTurn;
}

bool Board::checkGameEnd() {
    bool fullRow = false;
    int count;
    for(int i=0;i<boardSize;i++) {
        count = 0;
        for(int j=0;j<boardSize;j++) {
            if(mosaic[j][i]->getColour()==RED || mosaic[j][i]->getColour()==YELLOW || mosaic[j][i]->getColour()==DARKBLUE || 
            mosaic[j][i]->getColour()==LIGHTBLUE || mosaic[j][i]->getColour()==BLACK || mosaic[j][i]->getColour()==ORANGE) {
                count++;
            }
        }
        if(count==5) {
            fullRow = true;
        }
    }
    return fullRow;
}

bool Board::checkLowerCase(Tile* tile) {
    bool out = false;
    if(tile!=nullptr) {
        if(tile->getColour()==RED || tile->getColour()==YELLOW || tile->getColour()==DARKBLUE 
            || tile->getColour()==LIGHTBLUE || tile->getColour()==BLACK || tile->getColour()==ORANGE) {
            out = true;
        }
    }
    return out;
}