#ifndef ANIMATION_H
#define ANIMATION_H


class Animation{

public:

  virtual ~Animation(){};

  virtual void step(){};

  virtual bool isRunning(){
    return false;
  };
 

};

#endif
