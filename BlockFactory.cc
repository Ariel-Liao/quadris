#include "BlockFactory.h"
#include "BlockSubclasses.h"
#include <memory>
using namespace std;
// declare the singleton instance or else it doesn't compile
shared_ptr<BlockFactory> BlockFactory::instance;

BlockFactory::BlockFactory(){
    factoryMap['I']=(make_unique<IBlockFactory>());
    factoryMap['J']=(make_unique<JBlockFactory>());
    factoryMap['S']=(make_unique<SBlockFactory>());
    factoryMap['Z']=(make_unique<ZBlockFactory>());
    factoryMap['T']=(make_unique<TBlockFactory>());
    factoryMap['O']=(make_unique<OBlockFactory>());
    factoryMap['L']=(make_unique<LBlockFactory>());
    factoryMap['*']=(make_unique<OneBlockFactory>());
    factoryMap['?']=(make_unique<HintBlockFactory>());
}

std::shared_ptr<Block> BlockFactory::createBlock(char c, int level, Coordinate pivot){
    if(factoryMap.find(c) != factoryMap.end()){
        if(pivot.x >= 0 && pivot.y >= 0) {
           shared_ptr<Block> b = factoryMap[c]->create(level);
           b->setPivot(pivot);
           return b;
        }
        else return factoryMap[c]->create(level); // Use the default pivot
    }
    else{
        return nullptr;
    }
}

shared_ptr<BlockFactory> BlockFactory::getInstance(){
    if(instance == nullptr){
        instance = shared_ptr<BlockFactory>(new BlockFactory());
    }
    return instance;
}

std::shared_ptr<Block> IBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<IBlock>(level));
}
std::shared_ptr<Block> JBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<JBlock>(level));
}
std::shared_ptr<Block> SBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<SBlock>(level));
}
std::shared_ptr<Block> ZBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<ZBlock>(level));
}
std::shared_ptr<Block> TBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<TBlock>(level));
}
std::shared_ptr<Block> LBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<LBlock>(level));
}
std::shared_ptr<Block> OBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<OBlock>(level));
}
std::shared_ptr<Block> OneBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<OneBlock>(level));
}
std::shared_ptr<Block> HintBlockFactory::create(int level){
    return shared_ptr<Block>(make_shared<HintBlock>(level));
}