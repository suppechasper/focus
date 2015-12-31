#ifndef DISPLAYELEMENT_H
#define DISPLAYELEMENT_H

//include opengl stuff
#ifdef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <FTGL/ftgl.h>
#endif


#include <string>

class  DisplayElement{
  protected:

    bool db; 
    int offLeft, offRight, offTop, offBot;
    double x, y, w, h;
    int xLeft, yTop, width, height;
    int globalWidth, globalHeight;


  public:


    DisplayElement(){
      x = 0; 
      y = 0;
      w = 0;
      h = 0;
      offLeft= 5;
      offRight = 5;
      offBot = 5;
      offTop = 5;

      xLeft = 0;
      yTop = 0;
      width = 0;
      height = 0;
      db = true;

      globalWidth = 0;
      globalHeight = 0;


    };

    virtual ~DisplayElement(){};

    //Relative location 0 left/top, 1 right/bottom
    void location(double xx, double yy, double ww, double hh, int leftOffset =5,
        int rightOffset = 5, int topOffset = 5, int bottomOffset = 5){ 
      x = xx;
      y = yy;
      w = ww;
      h = hh;
      checkLocation();

      offLeft = leftOffset;
      offRight = rightOffset;
      offBot = bottomOffset;
      offTop = topOffset;
    };

    virtual void display() = 0;

    virtual void idle(){};

    virtual void reshape(int wi, int he){
      globalWidth = wi;
      globalHeight = he;
      xLeft = x * wi + offLeft;
      yTop = y * he + offTop;
      width = (int)(w*wi) - offLeft - offRight;
      height = (int)(h*he) - offTop - offBot;

    };

    bool drawBackground(){
      return db;
    };

    void setDrawBackground(bool b){
      db= b;
    };


    virtual void keyboard(unsigned char key, int x, int y) =0;

    virtual void special(int key, int xM, int yM) =0;

    virtual void mouse(int button, int state, int xM, int yM) = 0;

    virtual void motion(int xM, int yM) = 0;
    
    virtual void passive(int x, int y) = 0;


    bool isInside(int x, int y){
      return x>xLeft && x < (xLeft+width) &&
        y>yTop && y < (yTop+height) ;
    };

  protected:
    void checkLocation(){
      const static double eps= 0.01;
      if(x > 1-eps){
        x=1-eps;
      }
      else if( x < 0){
        x=0;
      }
      if(w < eps){
        w = eps;
      }
      if(x+w > 1){
        w = 1-x;
      }

      if(y>1-eps){
        y=1-eps;
      }
      else if(y<0){
        y=0;
      }
      if(h<eps){
        h=eps;
      }
      if(y+h > 1){
        h = 1-y;
      }

    };

};

#endif
