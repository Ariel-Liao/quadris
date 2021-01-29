#include "Pixel.h"

Pixel::Pixel(char c){
    val = c;
    block = nullptr;
    occupied = false;
}

void Pixel::clear(){
    val = ' ';
    if(block) block->incrementNumPixels(-1);
    block = nullptr;
    occupied = false;
}

void Pixel::setBlock(std::shared_ptr<Block> bl){
    val = bl->getLetter();
    bl->incrementNumPixels(1);
    block = bl;
    occupied = true;
}