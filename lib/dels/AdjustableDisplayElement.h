#ifndef ADJUSTABLEDISPLAYELEMENT_H
#define ADJUSTABLEDISPLAYELEMENT_H

#include <math.h>

class  AdjustableDisplayElement : public DisplayElement{
  private:

    bool moveAction;
    bool resizeAction;


  public:


    AdjustableDisplayElement(){
      moveAction=false;
      resizeAction=false;
    };

    virtual ~AdjustableDisplayElement(){};


    virtual void displayAction() = 0;


    virtual void display(){
      glColor3f(0.7, 0.7, 0.7);
      glLineWidth(1);
      glBegin(GL_LINES);
      glVertex2i(xLeft, yTop);
      glVertex2i(xLeft+5, yTop);
      glVertex2i(xLeft, yTop);
      glVertex2i(xLeft, yTop+5);
      
      glVertex2i(xLeft+width,   yTop+height);
      glVertex2i(xLeft+width-5, yTop+height);
      glVertex2i(xLeft+width,   yTop+height);
      glVertex2i(xLeft+width,   yTop+height-5);
      glEnd();

      displayAction();
    };



    virtual void mouseAction(int button, int state, int x, int y) = 0;


    virtual void mouse(int button, int state, int xM, int yM){

      if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        int xC1 =  xM-xLeft;
        int xC2 =  xLeft+width-xM;

        int yC1 =  yM-yTop;
        int yC2 =  yTop+height-yM;

        if( 0 <= xC1 && xC1 < 5   &&  0 <= yC1 && yC1 < 5){
          moveAction=true;
        }
        else if( 0 <= xC2 && xC2 < 5   &&  0 <= yC2 && yC2 < 5){
          resizeAction=true;
        }
        else{
          moveAction = false;
          resizeAction = false;
        }
      }        
      else{
        moveAction = false;
        resizeAction = false;
      }

      mouseAction(button, state, xM, yM);
    };


    virtual void motionAction(int  x, int y) = 0;


    virtual void motion(int xM, int yM){

      if(moveAction){
        if(xM < xLeft + width - offLeft - offRight){
          x = xM/ (double) globalWidth;  
          if(x+w > 1){
            w = 1-x;
          }
        }
        if(yM < yTop + height - offBot - offTop){
          y = yM/ (double) globalHeight; 
          if( y+h > 1){
            h = 1-y;
          } 
        }
        this->checkLocation();
        this->reshape( globalWidth, globalHeight );
        glutPostRedisplay();
      }      
      else if(resizeAction){

        if(xM > xLeft + offLeft + offRight){
          w = (xM-xLeft)/(double) globalWidth;
          if( x+w > 1 ){
            w = 1-x;
          }
        }
        if(yM > yTop + offBot + offTop){
          h = (yM-yTop)/(double) globalHeight;          
          if( y+h > 1 ){
            h = 1-y;
          }
        }
        this->checkLocation();
        this->reshape( globalWidth, globalHeight );
        glutPostRedisplay();
      }

      motionAction(xM, yM);

    };


};

#endif
