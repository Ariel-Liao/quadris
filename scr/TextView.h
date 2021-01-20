#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include <iostream>
#include "Observer.h"
#include "GameModel.h"

class TextView: public Observer{
    public:
        TextView(GameModel *);
        //~TextView(){};
        void update() override;
    private:
        GameModel* model;

};

#endif