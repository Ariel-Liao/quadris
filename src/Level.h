#ifndef LEVEL_H
#define LEVEL_H
#include "Block.h"
#include "RandomGenerator.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
class Block;

class Level{
    public:
        virtual std::shared_ptr<Block> nextBlock() = 0;
        //virtual ~Level(){};
        void setSequenceFromFile(std::string f);
        void setIsNoRandom(bool nr){norandom = nr;}
        void setRngSeed(int seed){rng->setSeed(seed);};
        // for level 4
        virtual void increase_(){}; // increase blockBeforeClear_ by 1
        virtual void decrease_(){}; // decrease blockBeforeClear_ by 1
        virtual void setblockBeforeClear_(int b){};
        virtual int getblockBeforeClear_(){return 0;};

    protected:
        bool norandom;
        std::vector<char> sequence;
        std::shared_ptr<RandomGenerator> rng;
        int index;
        // for level four
        int blockBeforeClear_ = 0;

};

class Level0: public Level{
    public:
    Level0(std::string sourceFile);
    std::shared_ptr<Block> nextBlock() override;
};
class Level1: public Level{
    public:
    Level1(std::shared_ptr<RandomGenerator>);
    std::shared_ptr<Block> nextBlock() override;
};
class Level2: public Level{
    public:
    Level2(std::shared_ptr<RandomGenerator>);
    std::shared_ptr<Block> nextBlock() override;
};
class Level3: public Level{
    public:
    Level3(std::shared_ptr<RandomGenerator>);
    std::shared_ptr<Block> nextBlock() override;
};
class Level4: public Level{
    public:
    Level4(std::shared_ptr<RandomGenerator>);
    std::shared_ptr<Block> nextBlock() override;
    // get count // set count // increase count
    void increase_() override; // increase blockBeforeClear_ by 1
    void decrease_() override; // decrease blockBeforeClear_ by 1
    void setblockBeforeClear_(int b) override;
    int getblockBeforeClear_() override;
    // private:
    // int blockBeforeClear_ = 0; // count
};

#endif
