#ifndef PROJECTIONBACKGROUND_H
#define PROJECTIONBACKGROUND_H


#include "ProjectionDEL.h"
#include "Projector.h"

template< typename TPrecision>
class ProjectionBackground{
  
  protected:
    
    RGB color;
    TPrecision alpha;
  
  public:

    ProjectionBackground(){
      color = RGB(0.5, 0.5, 0.5);
      alpha = 1;
    };
    
    virtual ~ProjectionBackground(){};

    void setColor(const RGB &col, TPrecision a){
      color=col;
      alpha = a;
    };

    virtual void drawBackground( ProjectionDEL<TPrecision> *del, Projector<TPrecision> *proj ) = 0;


};

#endif
