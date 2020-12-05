#include "Player.h"

Player::Player(std::string name){
    this->name = name;
    this->point = 0;
    this->board = nullptr;
}

Player::~Player(){
}

std::string Player::getName(){
    return name;
}

int Player::getPoint(){
    return point;
}

void Player::setPoints(int point) {
    if(point<0){
        this->point=0;
    }
    else{
        this->point=point;
    }
}

void Player::setBoard(Board* board){
    this->board = board;
}

Board* Player::getBoard(){
    return board;
}

