#ifndef LEVEL_FACTORY_H
#define LEVEL_FACTORY_H
#include "Level.h"

class BaseLevelFactory{
    public:
    //virtual ~BaseLevelFactory(){};
    virtual std::shared_ptr<Level> create(std::shared_ptr<RandomGenerator>, std::string sourceFile) = 0;
};

class LevelFactory{
    public:
    static std::shared_ptr<LevelFactory> getInstance(int s=0);  
    std::shared_ptr<Level> createLevel(int l, std::string sourceFile = "sequence.txt");
    private:
    LevelFactory(int s = 0);
    static std::shared_ptr<LevelFactory> instance;
    std::shared_ptr<RandomGenerator> rngPtr; // RNG is composition'd with Level Factory instead of each Level
    std::unordered_map<int, std::unique_ptr<BaseLevelFactory>> factoryMap;
};

class Level0Factory: public BaseLevelFactory{
    std::shared_ptr<Level> create(std::shared_ptr<RandomGenerator> = nullptr, std::string sourceFile = "sequence.txt") override;
};

class Level1Factory: public BaseLevelFactory{
    std::shared_ptr<Level> create(std::shared_ptr<RandomGenerator>, std::string sourceFile) override;
};

class Level2Factory: public BaseLevelFactory{
    std::shared_ptr<Level> create(std::shared_ptr<RandomGenerator>, std::string sourceFile) override;
};

class Level3Factory: public BaseLevelFactory{
    std::shared_ptr<Level> create(std::shared_ptr<RandomGenerator>, std::string sourceFile) override;
};
class Level4Factory: public BaseLevelFactory{
    std::shared_ptr<Level> create(std::shared_ptr<RandomGenerator>, std::string sourceFile) override;
};

#endif