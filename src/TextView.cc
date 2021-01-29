#include "TextView.h"

TextView::TextView(GameModel* g){
    model = g;
}
void TextView::update(){
    if(model){
        std::cout << "Level: " << model->getLevel() << std::endl;
        std::cout << "Score: " << model->getScore() << std::endl;
        std::cout << "Hi Score: " << model->getHiScore() << std::endl;
        std::cout << "-----------" << std::endl;
        for(int i =0; i < NUM_ROWS; i++){
            std::cout << (i+1);
            if(i+1 < 10) std::cout <<  " ";
            model->printRow(i);
        }
        std::cout << "-----------" << std::endl;
        std::cout << "Next:" << std::endl;
        model->printNextBlock();


    }
    if(model->isGameOver()){
      std::cout << "GAME OVER! Please restart the game." << std::endl;
    }
}
