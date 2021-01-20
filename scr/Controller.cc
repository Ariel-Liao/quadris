#include "Controller.h"
#include <iostream>
#include <sstream>
#include <string> 
#include <fstream>

Controller::Controller(GameModel* m, bool d){
    model = m;
    disableBonus = d;
    prevCommand = BAD_COMMAND;
    commandMap["left"] = Command::CMD_LEFT;
    commandMap["right"] = Command::CMD_RIGHT;
    commandMap["down"] = Command::CMD_DOWN;
    commandMap["clockwise"] = Command::CLOCKWISE;
    commandMap["counterclockwise"] = Command::COUNTERCLOCKWISE;
    commandMap["drop"] = Command::DROP;
    commandMap["levelup"] = Command::LEVELUP;
    commandMap["leveldown"] = Command::LEVELDOWN;
    commandMap["norandom"] = Command::NORANDOM;
    commandMap["random"] = Command::RANDOM;
    commandMap["sequence"] = Command::SEQUENCE;
    commandMap["restart"] = Command::RESTART;
    commandMap["I"] = Command::DBG_REPLACE_I;
    commandMap["J"] = Command::DBG_REPLACE_J;
    commandMap["S"] = Command::DBG_REPLACE_S;
    commandMap["Z"] = Command::DBG_REPLACE_Z;
    commandMap["L"] = Command::DBG_REPLACE_L;
    commandMap["O"] = Command::DBG_REPLACE_O;
    commandMap["T"] = Command::DBG_REPLACE_T;
    commandMap["rename"] = Command::RENAME;
    commandMap["hint"] = Command::HINT;
    commandMap["movetohint"] = Command::MOVE_BLOCK_TO_HINT;
    _terminated = false;

}

void Controller::renameCommand(std::string prev, std::string name){
    if(commandMap.find(name) != commandMap.end()) {
        std::cout << name << " already exists" << std::endl;
        return; 
    }// Name already exist
    Command prevCmd = checkPrefix(prev);
    if(prevCmd == BAD_COMMAND) {
        std::cout << prev << " is a bad command" << std::endl;
        return;}
    commandMap[name] = prevCmd;
    std::cout << "Renamed "<<prev<< " to "<< name << std::endl;
    // there will be edge case if you rename something with numbers like rename right 3fff :(
}

Command Controller::checkPrefix(std::string s){ // takes in "ri" and return RIGHT
    std::string res;
    if(commandMap.find(s)!= commandMap.end()) return commandMap[s]; // It exists in the cmd map
    for(int i = 0; i < s.length(); i++){
        int matches = 0;
        for(auto p: commandMap){
            std::string cmd = p.first;
            if(i < cmd.length() && s.substr(0, i+1) == cmd.substr(0, i+1)){
                res = cmd;
                matches++;
            }
        }
        if(matches == 1) return commandMap[res]; // Return when only one command matches the input
    }
    return BAD_COMMAND;
}

void Controller::runCommand(std::string cmd){ 
    if(_terminated) return;
	if(cmd.empty()) return;
    if(prevCommand == HINT){ // REMOVE THE HINT BLOCK on the next command
        model->clearHintBlock();
    }
   std::istringstream iss(cmd);
   std::string s;
   std::vector<std::string> tokens;
   int multiplier = 1;

	while(iss >> s) {
		tokens.push_back(s); // Split by token to get multi-word commands (norandom, sequence, rename)
   	}	 
  try{

    std::string::size_type sz;
    multiplier = std::stoi(tokens[0], &sz);
    tokens[0] = tokens[0].substr(sz);
  }catch(std::invalid_argument e){
    //No prefix, ignored.
  }
   Command enumCmd = checkPrefix(tokens[0]);

    switch(enumCmd){
        case(MOVE_BLOCK_TO_HINT):{
            if(model->isGameOver()) break;
            if(!disableBonus) model->dropCurBlockAtHint();
            else std::cout << "Bad command" << std::endl;
            break;
        }
        case(HINT):{
            if(model->isGameOver()) break;
            model->setHintBlockOnBoardForHintCommand();
            break;
        }
        case(RENAME):{
            // "rename a b" is a -> b
            if(disableBonus) std::cout << "Bad command" << std::endl;
            else{
                if(tokens.size()!= 3){
                    std::cout << "Bad rename format: should be rename <existing> <new>" << std::endl;
                }
                else renameCommand(tokens[1], tokens[2]);                
            }

            break;
        }
        case(DBG_REPLACE_I): //I
        case(DBG_REPLACE_J): //J
        case(DBG_REPLACE_S): //S
        case(DBG_REPLACE_Z): //Z
        case(DBG_REPLACE_L): //L
        case(DBG_REPLACE_O): //O
        case(DBG_REPLACE_T):{
            if(model->isGameOver()) break;
            model->replaceCurBlockWithType(tokens[0][0]);
            break;
        }
        case(CMD_RIGHT):{
            if(model->isGameOver()) break;
            model->moveCurBlockForCommand(RIGHT, multiplier); 
            break;}
        case(CMD_LEFT):{
            if(model->isGameOver()) break;
            model->moveCurBlockForCommand(LEFT, multiplier);
             break;}
        case(CMD_DOWN):{
            if(model->isGameOver()) break;
            model->moveCurBlockForCommand(DOWN, multiplier); 
            break;}
        case(CLOCKWISE):{
            if(model->isGameOver()) break;
            model->moveCurBlockForCommand(CW, multiplier);
            break;}
        case(COUNTERCLOCKWISE):{
            if(model->isGameOver()) break;
            model->moveCurBlockForCommand(CCW, multiplier);
            break;}
        case(DROP):{
            if(model->isGameOver()) break;
            model->dropCurBlock(multiplier);
            break;}
        case(LEVELUP): {
            if(model->isGameOver()) break;
            model->changeLevel(multiplier);
            break;}
        case(LEVELDOWN): {
            if(model->isGameOver()) break;
            model->changeLevel(-multiplier); 
            break;}
        case(RESTART): {model->restart(); break;}
        case(NORANDOM):{

            if(model->isGameOver()) break;
            if(tokens[1] == ""){
            	std::cout << "No filename" << std::endl;
            	break;
            }
            std::ifstream inFile;
            inFile.open(tokens[1]);
            if(!inFile) {
            	std::cout << "Invalid filename" << std::endl; 
            	inFile.close(); 
            	break;
            }
            model->setNoRandom(true, tokens[1]);
            inFile.close();
            break;               
        }
        case (RANDOM): {
            if(model->isGameOver()) break;
            model->setNoRandom(false, ""); 
            break;}
        case (SEQUENCE): {
            if(model->isGameOver()) break;
            if(tokens[1] == ""){
            	std::cout << "No filename" << std::endl;
            	break;
            }
            std::ifstream inFile;
            inFile.open(tokens[1]);
            if(!inFile) {
            	std::cout << "Invalid filename" << std::endl; 
            	inFile.close(); 
            	break;
            }
            
            std::string line;
            while(getline(inFile, line)){
                runCommand(line);
            }

            inFile.close();
            break;                 
        }

        default:{
            if(model->isGameOver()) break;
            std::cout << "Bad command" << std::endl;
            break;
        }
        
    }
    prevCommand = enumCmd;


}

void Controller::runWhileLoop(){
    std::string line;
    std::cout << ">";
    while(getline(std::cin, line)){
    	if(std::cin.eof()) {
            break;
        }
    	runCommand(line); 
    	std::cout << ">";
    }
    _terminated = true;
}
