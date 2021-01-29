#ifndef BLOCK_FACTORY_H
#define BLOCK_FACTORY_H
#include "Block.h"
#include <unordered_map>
#include <string>
#include <memory>


class BaseBlockFactory{ // Abstract Base BlockFactory
    public:
    virtual std::shared_ptr<Block> create(int level) = 0;
};

class BlockFactory{ // Singleton block factory that is used by Model
    public:
    static std::shared_ptr<BlockFactory> getInstance(); // singleton instance
    std::shared_ptr<Block> createBlock(char c, int level, Coordinate p = {-1,-1});
    private:
     BlockFactory();
    static std::shared_ptr<BlockFactory> instance; // singleton instance
    std::unordered_map<char, std::unique_ptr<BaseBlockFactory>> factoryMap;
};

class IBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};

class JBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};

class SBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};
class ZBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};
class TBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};
class LBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};
class OBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};
class OneBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};
class HintBlockFactory: public BaseBlockFactory{
    std::shared_ptr<Block>  create(int level) override;
};

#endif