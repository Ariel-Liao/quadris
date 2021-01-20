#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "GameModel.h"
#include <string>
#include <unordered_map>
#include <memory>
enum Command { 
    CMD_LEFT,
    CMD_RIGHT,
    CMD_DOWN,
    CLOCKWISE,
    COUNTERCLOCKWISE,
    DROP,
    LEVELUP,
    LEVELDOWN,
    NORANDOM,
    RANDOM,
    SEQUENCE,
    RESTART,
    HINT,
    // I,J, L, S, Z, O, T,
    DBG_REPLACE_I, // I
    DBG_REPLACE_J, // J
    DBG_REPLACE_L, // L
    DBG_REPLACE_S, // S
    DBG_REPLACE_Z, // Z
    DBG_REPLACE_O, // O
    DBG_REPLACE_T, // T
    //
    RENAME,
    MOVE_BLOCK_TO_HINT,
    BAD_COMMAND 
};

class Controller{
    public:
        Controller(GameModel* m, bool disableBonus);
        void renameCommand(std::string, std::string);

        void runWhileLoop();
        void runCommand(std::string);
    private:
        std::unordered_map<std::string, Command> commandMap;
        GameModel* model;
        Command checkPrefix(std::string);
        Command prevCommand;
        bool disableBonus;

        //used for multithread. When main loop is done, use this to disable manipulation from graphic view.
        //only useful for bonus feature
        bool _terminated;
};
#endif