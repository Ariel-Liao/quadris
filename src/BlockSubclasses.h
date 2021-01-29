#ifndef IBLOCK_H
#define IBLOCK_H
#include "Block.h"
#include <iostream>
// Top left of board for default position is 3,3 not 0,0 because 3 extra rows
// Actual coordinate is pivot + points[i]'s coordinate
class IBlock: public Block{
    public:
    IBlock(int level){
        pivot = Coordinate{3,0};
        points = std::vector<Coordinate>{{0,0}, {0,1}, {0,2}, {0,3}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'I';}
    void printBlock() override{
        std::cout << "IIII" << std::endl;
    }
};

class JBlock: public Block{
    public:
    JBlock(int level){
        pivot = Coordinate{4,0};
        points = std::vector<Coordinate>{{0,0}, {-1,0}, {0,1}, {0,2}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'J';}
    void printBlock() override{
        std::cout << "J\nJJJ" << std::endl;
    }
};

class SBlock: public Block{
    public:
    SBlock(int level){
        pivot = Coordinate{4,0};
        points = std::vector<Coordinate>{{0,0}, {0,1}, {-1,1}, {-1,2}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'S';}
    void printBlock() override{
        std::cout << " SS\nSS" << std::endl;
    }
};

class ZBlock: public Block{
    public:
    ZBlock(int level){
        pivot = Coordinate{4,0};
        points = std::vector<Coordinate>{{-1,1}, {-1,0}, {0,1}, {0,2}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'Z';}
    void printBlock() override{
        std::cout << "ZZ\n ZZ" << std::endl;
    }
};

class TBlock: public Block{
    public:
    TBlock(int level){
        pivot = Coordinate{4,0};
        points = std::vector<Coordinate>{{-1,0}, {0,1}, {-1,1}, {-1,2}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'T';}
    void printBlock() override{
        std::cout << "TTT\n T" << std::endl;
    }
};

class OBlock: public Block{
    public:
    OBlock(int level){
        pivot = Coordinate{4,0};
        points = std::vector<Coordinate>{{0,0}, {0,1}, {-1,1}, {-1,0}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'O';}
    void printBlock() override{
        std::cout << "OO\nOO" << std::endl;
    }
};

class LBlock: public Block{
    public:
    LBlock(int level){
        pivot = Coordinate{4,0};
        points = std::vector<Coordinate>{{0,0}, {0,1}, {0,2}, {-1,2}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 'L';}
    void printBlock() override{
        std::cout << "  L\nLLL" << std::endl;
    }
};

class OneBlock: public Block{
    public:
    OneBlock(int level = 4){
        pivot = Coordinate{4,5};
        points = std::vector<Coordinate>{{0,0}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return '*';}
    void printBlock() override{
        std::cout << "*" << std::endl;
    }
};

class HintBlock: public Block{
    public:
    HintBlock(int level = 0){
        pivot = Coordinate{0, 0};
        points = std::vector<Coordinate>{{0,0}};
        numPixelsRemaining =0;
        creationLevel = level;
        settled = false;
    };
    char getLetter() override {return 63;}
    void printBlock() override{
        std::cout << "????" << std::endl;
    }
};
#endif