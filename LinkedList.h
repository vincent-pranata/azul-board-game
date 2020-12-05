#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include <string>
#include <iostream>
#include <time.h>
#include <random>

class LinkedList
{
public:
   // Constructors
   LinkedList();
   // Deconstructors
    ~LinkedList();

   // Returns the size of this linked list
   int size();
   // Deletes every node within the linked list
   void clear();
   // Returns a particular node for the given index
   Node *get(int find);
   // Returns a particular tile for the given index
   Tile* getTile(int find);
   // Shorthand function that returns a tile, and removes it from
   // the list
   Tile* getAndRemoveTile(int index);
   // Removes a particular node within the list
   // NOTE: doesn't delete the node as there is little case to delete one tile
   void remove(int index);
   // Adds a Tile to the front of the linked list
   // This Replaces the head node
   void addFront(Tile *data);
   // Adds a Tile to the end of the linked list
   void addBack(Tile *data);
   // Changes the tile at a position in the list with the given tile
   // Careful... can lose the original pointer
   void setTile(int index, Tile *tile);
   // Shuffles the linked list, changing the order
   void shuffle();
   // Swaps to elements in a linked list with each other
   // element position i in linked list swaps with element position j
   void swap(int i, int j);
   // Prints the data of the linkedlist
   std::string printData();

   int getSeed();

   void setSeed(int number);

private:
   Node *head;
   int length;
   int min;
   int max;
   int seed;
};

#endif // ASSIGN2_LINKEDLIST_H