#include "Menu.h"

#define EXIT_SUCCESS 0

int main(int argc, char** argv){
   if(argc>=2){
      if(argv[argc-2][0]=='-' && argv[argc-2][1]=='s' ){
         if(isdigit((*argv[argc-1]))){
            Menu menu = Menu(std::stoi(argv[argc-1]));
         }
         std::cout << "Invalid seed, please use a number" << std::endl;
      }
      else{
         std::cout << "Please do './azul -s <seed>' or just do '.azul'" << std::endl;
      }
   }
   else if (argc==1){
      Menu menu = Menu();
   }
   else{
      std::cout << "Please do './azul -s <seed>' or just do '.azul'" << std::endl;
   }
   return EXIT_SUCCESS;
}