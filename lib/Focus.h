#ifndef FOCUS_H
#define FOCUS_H

#include "Eigen/Dense"

#include "Display.h"
#include "DataInterface.h"


#include "Animator.h"
#include "Rotation.h"
#include "DisplayElement.h"

#include <vector>

#include "Font.h"

#include <iostream>

bool whiteBackground = false;

template<typename TPrecision>
class Focus : public Display{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

#define BUFSIZE 512

    int width, height;
    int xM, yM;
    
    Font &font;
    


    Animator animator;
   
    std::vector<DisplayElement *> elements;

    RGB bgCol;
  public:
    

    Focus(Font &f) : font(f){
      width = 0;
      height = 0; 
      bgCol = RGB(0.1, 0.1, 0.1);
    };



    Animator &getAnimator(){
      return animator;
    };

    
    Font &getFont(){
      return font;
    };



    std::string title(){
      return "Focused Projections";
    };

    void reshape(){
      reshape(width, height);
    };

    void reshape(int w, int h){
      width = w;
      height = h;

      glViewport(0, 0, w, h);       
      glMatrixMode(GL_PROJECTION);  
      glLoadIdentity();
      glOrtho (0, width, height, 0, 0, 1);
      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->reshape(w, h);
      }
    };




    void clearDisplayElements(){
      for(std::vector<DisplayElement *>::iterator it = elements.begin(); it !=
          elements.end(); ++it){
        delete (*it);
      }
      elements.clear();
    };



    void addDisplayElement(DisplayElement *el){
      elements.push_back(el);
    };


    void setBackgroundColor(RGB &col){
      bgCol = col;
    };


    void init() {  
      
      glEnable(GL_POLYGON_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glEnable( GL_POINT_SMOOTH );
      glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      //glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    };



    void printHelp(){

      std::cout << "Projection Display" << std::endl;
      std::cout << "------------------" << std::endl;
      std::cout << "Up and down arrow to increase / decrease point size" << std::endl ; 
      std::cout << "Left and right arrow to adjust transparency" << std::endl ; 
      std::cout << "Press c to clear searched" << std::endl ; 
      std::cout << "Press + to increase selection circle" << std::endl ;
      std::cout << "Press - to decrease selection circle" << std::endl << std::endl;



    };






    void display(void){

      glClearColor(bgCol.r, bgCol.g, bgCol.b, 0);
      
      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();

      glClear(GL_COLOR_BUFFER_BIT);

      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->display();
      }

      glutSwapBuffers();

    };




    void idle(){
      animator.step();
    };





    void keyboard(unsigned char key, int x, int y){
      if(animator.isRunning()){
        return;
      }      
      switch(key){

        case 'h':
        case 'H':
          printHelp();
          break;

      }
      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->keyboard(key, x, y);
      }
      glutPostRedisplay();
    };





    void special(int key, int x, int y){
      if(animator.isRunning()){
        return;
      }

      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->special(key, x, y);
      }
      glutPostRedisplay();
    };






    void mouse(int button, int state, int x, int y){
      if(animator.isRunning()){
        return;
      }
      xM = x;
      yM = y;
      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->mouse(button, state, x, y);
      }
      glutPostRedisplay();

    };





    void motion(int x, int y){
      if(animator.isRunning()){
        return;
      }
      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->motion(x, y);
      }
      glutPostRedisplay();
    };






    void passive(int x, int y){
      if(animator.isRunning()){
        return;
      }
      xM = x;
      yM = y;

      for(std::vector<DisplayElement *>::iterator it=elements.begin(); 
          it != elements.end(); ++it){
        (*it)->passive(x, y);
      }
      glutPostRedisplay();

    };









};

#endif
