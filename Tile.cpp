#include "Tile.h"

Tile::Tile(){
    colour = NOTILE;
    eColour = ME_WHITE;
}

Tile::Tile(Colour colour){
    this->colour = colour;
    if(colour==RED){
        eColour = ME_RED;
    }
    else if(colour==YELLOW){
        eColour = ME_YELLOW;
    }
    else if(colour==DARKBLUE){
        eColour = ME_DARKBLUE;
    }
    else if(colour==LIGHTBLUE){
        eColour = ME_LIGHTBLUE;
    }
    else if(colour==BLACK){
        eColour = ME_BLACK;
    }
    else if(colour==ORANGE){
        eColour = ME_ORANGE;
    }
    else{
        eColour = ME_WHITE;
    }
}

Colour Tile::getColour() { 
    return colour; 
}

EnhancedColour Tile::getEColour() { 
    return eColour; 
}
