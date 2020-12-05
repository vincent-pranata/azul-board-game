#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <string>
#include "utils.h"

// Define a Colour type
typedef char Colour;
typedef std::string EnhancedColour;

class Tile
{
    public:
        Tile();
        Tile(Colour colour);
        Colour getColour(); 
        EnhancedColour getEColour(); 

    private:
        Colour colour;
        EnhancedColour eColour;
};

#endif // ASSIGN2_TILE_H