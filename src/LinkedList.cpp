#include "LinkedList.h"

LinkedList::LinkedList(){
   head = nullptr;
   length = 0;
}

LinkedList::~LinkedList(){
   // Work backwards so as to not delete the head first
   for (int i = size() - 1; i > -1; i--){
      delete get(i);
   }
}


Node *LinkedList::get(int find){
   Node *retVal = nullptr;
   Node *currentNode = head;
   for (int i = 0; i < size(); i++){
      if (i == find){
         retVal = currentNode;
      }
      if (currentNode->next){
         currentNode = currentNode->next;
      }
   }
   return retVal;
}

Tile *LinkedList::getTile(int find){
   Tile *retVal = nullptr;
   Node *currentNode = head;
   for (int i = 0; i < size(); i++){
      if (i == find){
         retVal = currentNode->tile;
      }
      if (currentNode->next){
         currentNode = currentNode->next;
      }
   }
   return retVal;
}

void LinkedList::addFront(Tile *data){
   if (head){
      Node *n = new Node(data, head);
      head = n;
   }
   else{
      Node *n = new Node(data, nullptr);
      head = n;
   }
   length++;
}

void LinkedList::addBack(Tile *data){
   Node *n = new Node(data, nullptr);
   // If there is no head for the list, set this new data as the head
   if (head){
      for (int i = 0; i < this->size(); i++){
         if (get(i)->next == nullptr){
            get(i)->next = n;
         }
      }
   }
   else{
      head = n;
   }
   length++;
}

Tile *LinkedList::getAndRemoveTile(int index){
   Tile *retTile = getTile(index);
   remove(index);
   return retTile;
}

void LinkedList::remove(int index){
   Node *node = get(index);
   if (head == node){
      if (node->next == nullptr){
         head = nullptr;
      }
      else{
         head = node->next;
      }
   }
   else{
      Node *prevNode = get(index - 1);
      prevNode->next = node->next;
   }
   length--;
}

int LinkedList::size(){
   return length;
}

void LinkedList::setTile(int index, Tile *tile){
   Node *node = get(index);
   node->tile = tile;
}

void LinkedList::shuffle(){
      int min = 0;
      int max = size();
      std::default_random_engine engine(seed);
      std::uniform_int_distribution<int> uniform_dist(min, max);
      srand(time(NULL));
      for (int i = 0; i < size(); i++){
         int r = uniform_dist(engine) % size();
         swap(i, r);
   }
}

void LinkedList::setSeed(int number) {
   seed = number;
}

int LinkedList::getSeed() {
   return seed;
}

void LinkedList::swap(int i, int j){
   Node *nodei = get(i);
   Node *prevNodei = get(i - 1);
   Node *nodej = get(j);
   Node *prevNodej = get(j - 1);

   Node *tempNode;

   // Make sure if the head node is selected to update the list
   if (head == nodei){
      head = nodej;
   }
   else if (head == nodej){
      head = nodei;
   }

   // Make sure the previous nodes point to the new next ones
   if (prevNodei){
      prevNodei->next = nodej;
   }
   if (prevNodej){
      prevNodej->next = nodei;
   }
   // swap the next attributes
   tempNode = nodei->next;
   nodei->next = nodej->next;
   nodej->next = tempNode;
}

std::string LinkedList::printData(){
   std::string data = "";
   for (int i = 0; i < size(); i++){
      data += get(i)->tile->getColour();
      if (i != size() - 1){
         data += " ";
      }
   }
   return data;
}
