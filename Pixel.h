#ifndef PIXEL_H
#define PIXEL_H
#include "Block.h"
#include <memory>

class Pixel{
    public:
        Pixel(char val = ' ');
        char getVal(){return val;}
        bool isOccupied(){return occupied;}
        std::shared_ptr<Block> getBlock(){return block;};

        void setBlock(std::shared_ptr<Block>); // Also set val and occupied
        void clear(); // ' ' and null
    private:
    char val;
    bool occupied;
    std::shared_ptr<Block> block;
};
#endif