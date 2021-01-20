#include "LevelFactory.h"
#include "Level.h"
using namespace std;

shared_ptr<LevelFactory> LevelFactory::instance;

LevelFactory::LevelFactory(int s){
    factoryMap[0] = make_unique<Level0Factory>();
    factoryMap[1] = make_unique<Level1Factory>();
    factoryMap[2] = make_unique<Level2Factory>();
    factoryMap[3] = make_unique<Level3Factory>();
    factoryMap[4] = make_unique<Level4Factory>();
    // new object
    rngPtr = make_shared<RandomGenerator>(s);
}

shared_ptr<Level> LevelFactory::createLevel(int l, string sourceFile){
    if(factoryMap.find(l) != factoryMap.end()){
        return factoryMap[l]->create(rngPtr, sourceFile);
    }
    else{
        return nullptr;
    }
}

shared_ptr<LevelFactory> LevelFactory::getInstance(int s){
    if(instance == nullptr){
        instance = shared_ptr<LevelFactory>(new LevelFactory(s));
    }
    return instance;
}
// lmao why did i make levels create the random generators...
shared_ptr<Level> Level0Factory::create(std::shared_ptr<RandomGenerator> r, string sourceFile){
    return make_shared<Level0>(sourceFile);
}
shared_ptr<Level> Level1Factory::create(std::shared_ptr<RandomGenerator> r, string sourceFile){
    return make_shared<Level1>(r);
}
shared_ptr<Level> Level2Factory::create(std::shared_ptr<RandomGenerator> r, string sourceFile){
    return make_shared<Level2>(r);
}
shared_ptr<Level> Level3Factory::create(std::shared_ptr<RandomGenerator> r, string sourceFile){
    return make_shared<Level3>(r);
}
shared_ptr<Level> Level4Factory::create(std::shared_ptr<RandomGenerator> r, string sourceFile){
    return make_shared<Level4>(r);
}