#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include "Subject.h"
#include "Block.h"
#include "LevelFactory.h"
#include "Pixel.h"

#include <mutex>
#include <shared_mutex>
#include <set>

#define NUM_COLS 11
#define NUM_ROWS 18
#define START_ROW 3
class GameModel: public Subject{
    public:
    GameModel(int s = 0, int l = 0, std::string sequenceFile = "sequence.txt");
    mutable std::recursive_mutex game_mutex;

    // Accessors
    int getLevel() const;
    int getScore() const;
    int getHiScore() const;
    bool isGameOver() const;
    std::vector<Coordinate> getNextBlockPoints() const;
    char getNextBlockLetter() const;
    std::vector<std::vector <Pixel> > getBoard() const;
    void setNoRandom(bool nr, std::string f = "");
    // Printing (for view)
    void printRow(int idx);
    void printNextBlock(); // For the "next" 
    // Commands
    void changeLevel(int increment);
    void replaceCurBlockWithType(char type); // For IJSZ... command
    // Hint/Clear Hint Command
    void clearHintBlock();
    void setHintBlockOnBoardForHintCommand();
    void dropCurBlockAtHint();
    void restart(); // Clear fields for restart command
    // Move Commands
    void moveCurBlockForCommand(MoveDirection, int); 
    void dropCurBlock(int multiplier = 1);
    //Initialize game
    void generateNextBlock();
    
    private:
        int level;
        int startLevel; // for restart
        int score;
        int hi_score;
        int seed;
        bool norandom; // only for level 3/4
        bool gameOver;
        std::string noRandomFileName;
        std::vector<std::vector <Pixel> > board; 
        std::shared_ptr<Level> levelStrategy;
        std::shared_ptr<Block> curBlock; 
        std::shared_ptr<Block> nextBlock;
        std::shared_ptr<Block> hintBlock;
        std::string _sequenceFile;
        // Score Helper Functions
        bool isRowRemovable(int i);
        void clearRowsUpdateScore();
        // Helper Functions for moving blocks
        // if this block's coordinates are not occupied
        bool isValidBlockPosition(std::shared_ptr<Block>); 
        // set the Pixels to point to the block, or clear the pixels
        void setBlockOnBoard(std::shared_ptr<Block>);
        void clearCoordinates(std::vector<Coordinate>);
        // modify the coordinates of the current block
        bool moveCurBlock(MoveDirection);
        bool rotateCurBlock(bool isCW);   
        // hint helper functions
        std::set<std::vector<Coordinate> > getPossibleDropPositions();
        float getWeightingForHintFromCoords(std::vector<Coordinate>);
        std::shared_ptr<Block> createHintBlock();
};

#endif
