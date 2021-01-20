#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
#define MAX_DIRECTION 5 // MAX_DIRS -x to get opposite direction
struct Coordinate{
    int x;
    int y;
};

bool operator==(const Coordinate& lhs, const Coordinate& rhs);
bool operator!=(const Coordinate& lhs, const Coordinate& rhs);
bool operator<(const Coordinate& lhs, const Coordinate& rhs);

enum MoveDirection{
    CW, LEFT, DOWN, UP, RIGHT, CCW
};


class Block{
    public:
        void rotate(bool isCW);
        void move(MoveDirection);
        //Accessors
        std::vector<Coordinate> getCoords();
        std::vector<Coordinate> getPoints() {return points;}; 
        Coordinate getPivot(){return pivot;}
        virtual char getLetter() = 0;
        int getNumPixels(){return numPixelsRemaining;}
        int getCreatedLevel(){return creationLevel;}
        //Mutators
        void setPointsFromAbsoluteCoords(std::vector<Coordinate>);
        void setPivot(Coordinate c){pivot = c;};
        void setSettled(bool b){settled = b;}
        void incrementNumPixels(int i){numPixelsRemaining += i;}
        // for printing in the "next" of the textview
        virtual void printBlock() = 0; 
        virtual ~Block(){};
    protected:
        Coordinate pivot;
        std::vector<Coordinate> points;
        int creationLevel;
        bool settled;
        int numPixelsRemaining; // increase when placed on board, decrease when cleared a pixel
};
#endif