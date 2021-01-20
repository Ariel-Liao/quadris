#include "Level.h"
#include <fstream>
#include <iostream>
#include "BlockFactory.h"
using namespace std;


void Level::setSequenceFromFile(string filename){
    sequence.clear();
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        std::cerr << filename << " not found!" << endl;
        return;
    }
    char x;
    while(inFile >> x){
        sequence.push_back(x);
    }
    inFile.close();
}

Level0::Level0(std::string sourceFile){
    index = 0;
    setSequenceFromFile(sourceFile); // Level 0 only has index and sequence
}

shared_ptr<Block> Level0::nextBlock(){
    char blockType = sequence[index % sequence.size()];
    index = (index+1) % sequence.size();
    return BlockFactory::getInstance()->createBlock(blockType, 0);
}

Level1::Level1(std::shared_ptr<RandomGenerator> r){
    rng = r; // Level 1 has no norandom/sequence/index
}

shared_ptr<Block> Level1::nextBlock(){
    int x = rng->rand_int(0, 12);
    char blockType;
    if(x <= 0) blockType = 'S';
    else if(x <= 1) blockType = 'Z';
    else if(x <= 3) blockType = 'I';
    else if(x <= 5) blockType = 'J';
    else if(x <= 7) blockType = 'T';
    else if(x <= 9) blockType = 'O';
    else blockType = 'L';
    return BlockFactory::getInstance()->createBlock(blockType, 1);
}

Level2::Level2(std::shared_ptr<RandomGenerator> r){
    rng = r;
    // Level 2 also only has random option
}

shared_ptr<Block> Level2::nextBlock(){
    char blockType;

    int x = rng->rand_int(0, 7);
    switch(x){
        case 0: {blockType = 'S'; break;}
        case 1: {blockType = 'Z'; break;}
        case 2: {blockType = 'I'; break;}
        case 3: {blockType = 'J'; break;}
        case 4: {blockType = 'T'; break;}
        case 5: {blockType = 'O'; break;}
        default :{ blockType = 'L'; break;}
    }

    return BlockFactory::getInstance()->createBlock(blockType, 2);
}

Level3::Level3(std::shared_ptr<RandomGenerator> r){
    rng = r;
    norandom = false;
    index = 0;
    // the factory will use other functions to set norandom/sequence
}

shared_ptr<Block> Level3::nextBlock(){
    char blockType;
    if(norandom && !sequence.empty()){
        blockType = sequence[index];
        index = (index+1) % sequence.size();
    }
    else{
       int x = rng->rand_int(0, 9);
        if(x <= 1) blockType = 'S';
        else if(x <= 3) blockType = 'Z';
        else if(x <= 4) blockType = 'T';
        else if(x <= 5) blockType = 'L';
        else if(x <= 6) blockType = 'I';
        else if(x <= 7) blockType = 'J';
        else blockType = 'O';
    }
   
    return BlockFactory::getInstance()->createBlock(blockType, 3);
}


// for level 4, we add the droprow based on level 3
Level4::Level4(std::shared_ptr<RandomGenerator> r){
    rng = r;
    norandom = false;
    index = 0;
    // the factory will use other functions to set norandom/sequence
    blockBeforeClear_ = 1;
}

void Level4::increase_() {
    blockBeforeClear_ += 1;
}

void Level4::decrease_() {
    blockBeforeClear_ -= 1;
}

void Level4::setblockBeforeClear_(int b) {
    blockBeforeClear_ = b;
}

int Level4::getblockBeforeClear_() {
    return blockBeforeClear_;
}


shared_ptr<Block> Level4::nextBlock() {
    char blockType;
    if(norandom && !sequence.empty()){
        blockType = sequence[index];
        index = (index+1) % sequence.size();
    }
    else{
       int x = rng->rand_int(0, 9);
        if(x <= 1) blockType = 'S';
        else if(x <= 3) blockType = 'Z';
        else if(x <= 4) blockType = 'T';
        else if(x <= 5) blockType = 'L';
        else if(x <= 6) blockType = 'I';
        else if(x <= 7) blockType = 'J';
        else blockType = 'O';
    }

    blockBeforeClear_++;
    return BlockFactory::getInstance()->createBlock(blockType, 4);
}




