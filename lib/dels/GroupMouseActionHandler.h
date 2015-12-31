#ifndef GROUPMOUSEACTIONHANDLER_H
#define GROUPMOUSEACTIONHANDLER_H


template<typename TPrecision>
class GroupMouseActionHandler{

  private:

    std::vector<MouseActionHandler<TPrecision> *> handlers;


  public:
  
    virtual void mouseAction(int button, int state, int x, int y){
      for(int i=0; i<handlers.size(); i++){
       handlers[i].mouseAction( button, state, x, y );
      } 
    };

};





#endif
