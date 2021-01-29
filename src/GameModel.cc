#include "GameModel.h"
#include "BlockFactory.h"
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <set>
#include <algorithm>
using namespace std;

GameModel::GameModel(int s, int l, std::string sequenceFile){
    level = l;
    startLevel=l;
    score = 0;
    hi_score=0;
    norandom = false;
    seed = s; 
    _sequenceFile = sequenceFile;
    levelStrategy = LevelFactory::getInstance(s)->createLevel(l, _sequenceFile);
    board = vector<vector<Pixel> >(NUM_ROWS, vector<Pixel>(NUM_COLS));
    gameOver = false;
    hintBlock = nullptr;
}

void GameModel::restart(){
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    level = startLevel;
    score = 0; // Keep the hiscore
    norandom = false;
    board = vector<vector<Pixel> >(NUM_ROWS, vector<Pixel>(NUM_COLS));
    levelStrategy = LevelFactory::getInstance(seed)->createLevel(startLevel, _sequenceFile);
    gameOver = false;
    curBlock = nullptr;
    nextBlock = nullptr;
    hintBlock = nullptr;
    generateNextBlock(); // This will notify if cur and next were null
}

void GameModel::clearHintBlock(){ // Remove hint block from board
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    if(hintBlock == nullptr) return;
    clearCoordinates(hintBlock->getCoords());
    hintBlock = nullptr;
    notify();
}

shared_ptr<Block> GameModel::createHintBlock(){ 
// Compare possible drop locations, and return the hint block
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    // CLEAR THE CUR BLOCK OR EVERYTHING IS NOT VALID
    clearCoordinates(curBlock->getCoords());
    set<vector<Coordinate>> coords = getPossibleDropPositions();
    vector<Coordinate> maxcoords;
    float maxWeight = -1000000.0;
    for(vector<Coordinate> v: coords){
        float f = getWeightingForHintFromCoords(v);
        if(f > maxWeight){
            maxcoords = v;
            maxWeight = f;
        }
    }

    setBlockOnBoard(curBlock); // Cleared it at the start, now set it again
    hintBlock = BlockFactory::getInstance()->createBlock('?', curBlock->getCreatedLevel());
    hintBlock ->setPointsFromAbsoluteCoords(maxcoords);
    return hintBlock;
}

void GameModel::setHintBlockOnBoardForHintCommand(){
    // Calculates the hint block and places it on the board
    if(gameOver) return; 
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    shared_ptr<Block> hb = createHintBlock();
    setBlockOnBoard(hintBlock);
    notify();
}
void GameModel::dropCurBlockAtHint(){
    // Drops the current block at the hint block location
    if(gameOver) return; 
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    shared_ptr<Block> hb = createHintBlock();
    clearCoordinates(curBlock->getCoords());
    curBlock->setPointsFromAbsoluteCoords(hb->getCoords());
    setBlockOnBoard(curBlock);
    dropCurBlock();
}

float GameModel::getWeightingForHintFromCoords(vector<Coordinate> points){
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    int numCleared = 0;
    vector<int> heights(NUM_COLS, 0);
    int numHoles = 0;

    for(int i = START_ROW; i < NUM_ROWS; i++){
        bool clearable = true;
        for(int j = 0; j < NUM_COLS; j++){
            if(!board[i][j].isOccupied() && 
            find(points.begin(), points.end(), Coordinate({i, j})) == points.end()){
                clearable = false;
                if(board[i-1][j].isOccupied() || 
                find(points.begin(), points.end(), Coordinate({i-1, j})) != points.end())
                    numHoles++; // Current is empty, and the one above is not empty
            }
            else if(heights[j] == 0){ // "else if" -> Current is not empty
                heights[j] = NUM_ROWS - i; 
            }
        }
        if(clearable) numCleared++;        
    }
    int total_height = 0;
    int bumpy = 0;
    for(int i = 0; i < NUM_COLS; i++){
        total_height += heights[i];
        if(i > 0) bumpy += abs(heights[i] - heights[i-1]);
    }
    // Algorithm from https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/?fbclid=IwAR2LBnZuaau1uO0H4lNIzxFsl29U6_B27XVUZtfD_H_lIcWaosHrnYVbtrA
    return -0.510066*total_height + 0.760666 * numCleared - 0.35663 * numHoles - 0.184483 * bumpy;
}

set<vector<Coordinate>> GameModel::getPossibleDropPositions(){
    // Get all coordinates of possible drop positions, for each column and orientation
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    // Create a copy of the cur block (same letter, same level, same pivot)
    shared_ptr<Block> temp = BlockFactory::getInstance()->createBlock(
        curBlock->getLetter(), curBlock->getCreatedLevel(), curBlock->getPivot());
    int tmpLvl = curBlock -> getCreatedLevel();
    Coordinate tmpPivot = curBlock->getPivot();
    
    set<vector<Coordinate>> allDropCoordinates;
    for(int rotation = 0; rotation < 4; rotation++){
        temp->rotate(true); // 0 = right 1 = down 2 = left 3 = up

        int start_col = tmpPivot.y;
        for(int col = start_col; col < NUM_COLS; col++){
            // If the created lvl is >= 3, the start pivots 
            // moves 2 down if you need rotate + leftright, 1 down if only leftright move
            int start_row = tmpPivot.x;
            if(tmpLvl >= 3){
                if(rotation != 3 && col != start_col)start_row = tmpPivot.x + 2;
                else if(rotation != 3 || col != start_col) start_row = tmpPivot.x + 1;
            }

            temp->setPivot({start_row, col});
            if(!isValidBlockPosition(temp)){break;}

            Coordinate lastPivot = {start_row, col};

            for(int row = start_row; row < NUM_ROWS; row++){
                temp->setPivot({row, col});
                if(!isValidBlockPosition(temp)) {break;}
                lastPivot.x = row;
            }
            temp->setPivot(lastPivot);
            allDropCoordinates.insert(temp->getCoords());
        }
        for(int col = start_col-1; col >= 0; col--){
            int start_row = tmpPivot.x;
            if(tmpLvl >= 3){
                if(rotation != 3 && col != start_col)start_row = tmpPivot.x + 2;
                else if(rotation != 3 || col != start_col) start_row = tmpPivot.x + 1;
            }
            
            temp->setPivot({start_row, col});
            if(!isValidBlockPosition(temp)) break;

            Coordinate lastPivot = {start_row, col};

            for(int row = start_row; row < NUM_ROWS; row++){
                temp->setPivot({row, col});
                if(!isValidBlockPosition(temp)) break;
                lastPivot.x = row;
            }
            temp->setPivot(lastPivot);
            allDropCoordinates.insert(temp->getCoords());
        }
    }

    return allDropCoordinates;
}


void GameModel::printRow(int idx){ // to help with printing row numbers for textview
    std::lock_guard<std::recursive_mutex> slk(game_mutex);
    if(idx >= 0 && idx < NUM_ROWS) {
        for(Pixel p: board[idx]) cout << p.getVal();
    }
    cout << endl;
}

void GameModel::printNextBlock(){
    std::lock_guard<std::recursive_mutex> slk(game_mutex);
    nextBlock->printBlock();
}

void GameModel::generateNextBlock(){
    // Sets the curBlock and nextBlock fields using levelStrategy
    // increases level4 count if is level4
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    bool iffirst = false;
    if(curBlock == nextBlock && nextBlock == nullptr) { // init
        curBlock = levelStrategy->nextBlock();
        iffirst = true;
    } else {
        curBlock = nextBlock;
    }
    nextBlock = levelStrategy->nextBlock();
    setBlockOnBoard(curBlock);
    if(iffirst) {
        notify();
    }
    
    if(level == 4) {
        levelStrategy->increase_();
    }
}

void GameModel::setBlockOnBoard(shared_ptr<Block> b){ // Updates the board to place the block.
    vector<Coordinate> points = b->getCoords();
    for(Coordinate c: points){
        board[c.x][c.y].setBlock(b);
    }
}

void GameModel::clearCoordinates(vector<Coordinate> points ){ // Clears pixels occupied by block
     for(Coordinate c: points){
        board[c.x][c.y].clear();
    }
}

void GameModel::replaceCurBlockWithType(char type){
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    if(gameOver) return;
    Coordinate pivot = curBlock->getPivot();
    shared_ptr<Block> newBlock = BlockFactory::getInstance()->createBlock(type, level, pivot);
    clearCoordinates(curBlock->getCoords()); // Always clear before check valid
    if(isValidBlockPosition(newBlock)){
        curBlock = newBlock;
            setBlockOnBoard(curBlock);
            notify(); // Only notify if you did a replace
    }else{
        setBlockOnBoard(curBlock);
    }
}

void GameModel::setNoRandom(bool nr, string f){
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    if(gameOver) return;
    // if level isn't >= 3, do nothing
    if(level <3) return;
    if(nr){
        norandom = nr;
        noRandomFileName = f;
        levelStrategy->setSequenceFromFile(f);
        levelStrategy->setIsNoRandom(true);
    }else if(!nr){
        norandom = nr;
        levelStrategy->setIsNoRandom(false);
    }
}

void GameModel::changeLevel(int increment){ // Increments level by specified amount (+1 or -1)
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    if(gameOver) return;
    int nl = level + increment;
    if(nl < 0 || nl > 4) return; 
    shared_ptr<Level> lptr = LevelFactory::getInstance()->createLevel(nl, _sequenceFile);
    if(lptr){
      levelStrategy = lptr;  
      level = nl;
      if(nl >= 3 && norandom){
          levelStrategy->setSequenceFromFile(noRandomFileName);
          levelStrategy->setIsNoRandom(true);
      }
      notify();
    } 
}

bool GameModel::isValidBlockPosition(std::shared_ptr<Block> b){
    vector<Coordinate> coords = b->getCoords();
    for(Coordinate c: coords){
        if(c.x < 0 || c.x >= NUM_ROWS || c.y < 0 ||
         c.y >= NUM_COLS || board[c.x][c.y].isOccupied()) {return false;}
    }
    return true;
}

bool GameModel::moveCurBlock(MoveDirection d){
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    // Opposite MoveDirection is max_dir-x (order is left, down, up, right)
    vector<Coordinate> oldPoints = curBlock->getCoords();
    curBlock->move(d);
    clearCoordinates(oldPoints); // CLEAR BEFORE YOU CHECK VALID OR IT CONFLICTS ITSELF
    bool res = true;
    if(!isValidBlockPosition(curBlock)){
         curBlock->move(static_cast<MoveDirection>(MAX_DIRECTION-d));
         res = false;
    }
    setBlockOnBoard(curBlock);
    return res;
}

bool GameModel::rotateCurBlock(bool isCW){
    vector<Coordinate> oldPoints = curBlock->getCoords();
    curBlock->rotate(isCW);
    clearCoordinates(oldPoints);
    bool res = true;
    if(!isValidBlockPosition(curBlock)){
         curBlock->rotate(!isCW);
         res = false;
    }
    setBlockOnBoard(curBlock);
    return res;
}

// Call this one from the controller. Also moves down heavy blocks
void GameModel::moveCurBlockForCommand(MoveDirection d, int multiplier){
    if(gameOver) return;
    bool res = false;
    for(int i = 0; i < multiplier; i++)
    {
        if(d == CW || d == CCW){
            res |= rotateCurBlock(d == CW);
        }else{
            res |= moveCurBlock(d);
        }
    }
    if(curBlock->getCreatedLevel() >= 3){
        res |= moveCurBlock(DOWN); 
    }
    if(res){
      notify(); // Moved something, notify observer
    }
}
// Drop command
// repeat how many times
void GameModel::dropCurBlock(int multiplier){
    std::unique_lock<std::recursive_mutex> ulk(game_mutex);
    if(gameOver) return;
    for(int j = 0; j < multiplier; j++){
        for(int i = 0; i < 14; i++){
            bool b = moveCurBlock(DOWN);
            if(!b) break;
        }
        curBlock->setSettled(true);

        // only check clear rows if drop 
        clearRowsUpdateScore();
        // place next block after, because if you clear rows it inserts empty rows
        if(!isValidBlockPosition(nextBlock)){
            gameOver = true;
        }
        // if level = 4, count satisfies the multiply of 5
        int count = levelStrategy->getblockBeforeClear_();
        if(level == 4 && count % 5 == 0 && count != 0) {
            curBlock = BlockFactory::getInstance()->createBlock('*', 4);
            // place next block after, because if you clear rows it inserts empty rows
            if(!isValidBlockPosition(curBlock)){
                gameOver = true;
            }
            levelStrategy->setblockBeforeClear_(1);
            dropCurBlock(1);
        } else {
            generateNextBlock();
        }
        levelStrategy->decrease_();
    }
    notify();
}

bool GameModel::isRowRemovable(int idx){
    if(idx < 3 || idx >= NUM_ROWS) return false;
    for(Pixel p: board[idx]){
        if(!p.isOccupied()) return false;
    }
    return true;
}
void GameModel::clearRowsUpdateScore(){
    int blockRemovedScore = 0;
    int numRowsCleared = 0;
    for(int i = START_ROW; i < NUM_ROWS; i++){ // increasing index, to avoid index problems when erase
        if(isRowRemovable(i)){
            numRowsCleared++;
            for(int j = 0; j < NUM_COLS; j++){
                shared_ptr<Block> bl = board[i][j].getBlock();
                board[i][j].clear(); // This should decrement num pixels and set block to null
                if(bl->getNumPixels() <= 0){
                    blockRemovedScore += (bl->getCreatedLevel()+1)*(bl->getCreatedLevel()+1);
                }
            }
            // Remove the row from the board
            board.erase(board.begin() + i);
            // Insert a new row at the start
            board.insert(board.begin(), vector<Pixel>(NUM_COLS));           
        }
    }
    if(numRowsCleared > 0){
        score += (level + numRowsCleared)*(level + numRowsCleared)+blockRemovedScore;
        hi_score = max(hi_score, score);  
        // if the level is four, set the count to zero 
        if(level == 4) {
            levelStrategy->setblockBeforeClear_(0);
        }     
    }

}
// Accessors
int GameModel::getLevel() const{
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return level;
}
int GameModel::getScore() const{
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return score;
}
int GameModel::getHiScore() const{
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return hi_score;
}
bool GameModel::isGameOver() const{
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return gameOver;
}
std::vector<Coordinate> GameModel::getNextBlockPoints() const{
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return nextBlock->getPoints();
}
char GameModel::getNextBlockLetter() const{
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return nextBlock->getLetter();
}
std::vector<std::vector <Pixel> > GameModel::getBoard() const {
  std::lock_guard<std::recursive_mutex> slk(game_mutex);
  return board;
}
