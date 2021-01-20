#ifndef SUBJECT_H
#define SUBJECT_H
#include "Observer.h"
#include <vector>
class Subject{
    public:
        Subject();
        void subscribe(Observer*);
        void unsubscribe(Observer*);
        void notify();
    private:
        std::vector<Observer*> observers;
};
#endif