#include "Block.h"
using namespace std;
bool operator==(const Coordinate& lhs, const Coordinate& rhs){
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
bool operator!=(const Coordinate& lhs, const Coordinate& rhs){
    return !(lhs == rhs);
}
bool operator<(const Coordinate& lhs, const Coordinate& rhs){
    if(lhs.x != rhs.x) return lhs.x < rhs.x;
    else return lhs.y < rhs.y;
}

void Block::setPointsFromAbsoluteCoords(vector<Coordinate> newCoords){ // Set block's pivot and points fields
    // Find the new bottom left
    int max_x = newCoords[0].x; // bottom x is higher number
    int min_y = newCoords[0].y;
    for(int i = 0; i < newCoords.size(); i++){
        max_x = max(max_x, newCoords[i].x);
        min_y = min(min_y, newCoords[i].y);
    }
    if(!points.empty()) points.clear();
    pivot = {max_x, min_y};
    

    for(int i = 0; i < newCoords.size(); i++){
        points.push_back({newCoords[i].x - max_x, newCoords[i].y - min_y});
    }
}
void Block::rotate(bool isCW){ // modify block's points field
    // points are relative to the pivot
    // Actual coordinate is pivot + points[i]
    if(!isCW){
        for(int i = 0; i < points.size(); i++){
            // First do y=x, x=-y
            int tmp = points[i].x;
            points[i].x=-1*points[i].y;
            points[i].y = tmp;
        }
    }else{
        for(int i = 0; i < points.size(); i++){
            // First do y=x, x=-y
            int tmp = points[i].x;
            points[i].x=points[i].y;
            points[i].y = -1*tmp;
        }

    }
    // Find the new bottom left and shift it
    int max_x = points[0].x; // bottom x is higher number
    int min_y = points[0].y;
    for(int i = 0; i < points.size(); i++){
        max_x = max(max_x, points[i].x);
        min_y = min(min_y, points[i].y);
    }
    // The "new bottom left" should be 0,0
    // coord + (0,0 - new bottom left)
    for(int i = 0; i < points.size(); i++){
        points[i].x -= max_x;
        points[i].y -= min_y;
    }
}
void Block::move(MoveDirection d){
    switch(d){
        case LEFT: {pivot.y--; break;}
        case RIGHT: {pivot.y++; break;}
        case DOWN: {pivot.x++; break;}
        case UP: {pivot.x--; break;}
    }
}

vector<Coordinate> Block::getCoords(){ // Absolute coordinates on board
    vector<Coordinate> res;
    for(Coordinate c: points){
        res.push_back(Coordinate{c.x+pivot.x, c.y+pivot.y});
    }
    return res;
}