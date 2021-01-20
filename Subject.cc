#include "Subject.h"
using namespace std;
Subject::Subject() {}

void Subject::subscribe(Observer* obs){
    observers.emplace_back(obs);
}
void Subject::unsubscribe(Observer* obs){
  for (auto it = observers.begin(); it != observers.end();) {
    if (*it == obs) {
      observers.erase(it);
    } else { ++it; }
  }
}
void Subject::notify(){
  for (int i = 0; i < observers.size(); ++i){
    observers[i]->update();
  }
}