#ifndef ANIMATIOR_H
#define ANIMATIOR_H

#include "Animation.h"
#include "DummyAnimation.h"

class Animator{
private:
  std::vector< Animation*> active;

public:

  Animator(){
  };

  void addAnimation(Animation *a){
    active.push_back(a);  
  };


  bool isRunning(){
   return active.size() > 0;
  };
  

  void step(){
    if(active.size() > 0){
      std::vector<Animation*> newActive;
      for(unsigned int i=0; i < active.size(); i++){
        active[i]->step();
        if( active[i]->isRunning() ){
          newActive.push_back(active[i]);
        }
      }
      active = newActive;
    }
  }; 

};

#endif
