#ifndef SCATTER3DEL_H
#define SCATTER3DEL_H


#include <vector>

#include "Eigen/Dense"

#include "DataInterface.h"

#include "AdjustableDisplayElement.h"

#include "Font.h"



template<typename TPrecision>
class Scatter3DEL : public AdjustableDisplayElement{

  protected:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


#define BUFSIZE 512

    Data<TPrecision> &data;

    
    //mouse 
    int last_x;
    int last_y;
    int cur_button;
    int mod;

    //naviagtion
    TPrecision zoom ;
    TPrecision tx ;
    TPrecision ty ;
    TPrecision tz ;
    TPrecision rotation[3];
    int rotationAxis ;


    float alpha;
    float pointSize;



  public:

    Scatter3DEL(Data<TPrecision> &d ) 
      : data( d ) { 

        pointSize = 6;

        mod = 0;
        cur_button = -1;

        //naviagtion
        zoom = 1;
        tx = 0;
        ty = 0;
        tz = 1;
        rotation[0] = -90;
        rotation[1] = 0;
        rotation[2] = 0;
        rotationAxis = 0;
      
    };


    virtual ~Scatter3DEL(){
    };


    virtual void reshape(int wi, int he){
      AdjustableDisplayElement::reshape(wi, he);
    };




    virtual void displayAction(void){

      using namespace Eigen;


      glViewport(this->xLeft, this->yTop, this->width, this->height);

      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();

      glScalef(zoom, zoom, zoom);
      glTranslatef(tx,ty, tz); 
      glRotatef(rotation[0], 1, 0, 0); 
      glRotatef(rotation[1], 0, 1, 0); 
      glRotatef(rotation[2], 0, 0, 1); 
      glTranslatef(0, 0, 0.5); 

      glPointSize(this->pointSize);
      glBegin(GL_POINTS);
      for(int i=0; i< data.getNumberOfPoints(); i++){
        const VectorXp &p= data.getData(i);

        double a;
        if(this->data.isActive(i) ){
          a = 0.1 + 0.9 * this->alpha * this->data.getDensity(i);
        }
        else{
          a = this->alpha * 0.1 * this->data.getDensity(i);
        }

        RGB &col = this->data.getColor(i);
        glColor4f(col.r, col.g, col.b, a);
        glVertex3f( p(0), p(1), p(3) );

      }
      glEnd();


    };





    virtual void keyboard(unsigned char key, int x, int y){
      switch(key){
        case 'x':
        case 'X':
          rotationAxis = 0;
          break;
        case 'y':
        case 'Y':
          rotationAxis = 1;
          break;
        case 'z':
        case 'Z':
          rotationAxis = 2;
          break;
      }
      glutPostRedisplay();
    };



    virtual void special(int key, int x, int y){
      switch(key)
      {
        case GLUT_KEY_LEFT:
          alpha *= 0.95;
          break;
        case GLUT_KEY_RIGHT:
          alpha *= 1.05;
          break;
        case GLUT_KEY_DOWN:
          pointSize *= 0.95;
          break;
        case GLUT_KEY_UP:
          pointSize *= 1.05;
          break;

      }
      if(alpha < 0){
        alpha=0;
      }
      else if(alpha > 1){
        alpha=1;
      }

      glutPostRedisplay();
    };






    virtual void mouseAction(int button, int state, int x, int y){
      last_x = x;
      last_y = y;
      mod = glutGetModifiers();
      if (state == GLUT_DOWN){
        if(button == GLUT_LEFT_BUTTON && (mod == GLUT_ACTIVE_CTRL || mod  == GLUT_ACTIVE_SHIFT)){
          cur_button = button;
        }
        else{
          cur_button = button;
        }
      }
      else if (button == cur_button){
        cur_button = -1;
      }
    };


    virtual void passive(int x, int y){

    };


    virtual void motionAction(int x, int y){
      int dx = x-last_x;
      int dy = y-last_y;

      switch(cur_button){
        case GLUT_LEFT_BUTTON:
          if(mod == GLUT_ACTIVE_CTRL){
            zoom += dx*0.01;
            if(zoom < 0){
              zoom = 0;
            }
          }
          else{
            // translate
            tx -= dx*0.01;
            ty += dy*0.01;  
          }
          break;

        case GLUT_MIDDLE_BUTTON:

          zoom += dx*0.01;
          if(zoom < 0){
            zoom = 0;
          }

          break;

        case GLUT_RIGHT_BUTTON:
          // rotate

          if(mod == GLUT_ACTIVE_CTRL){
          }
          else{
            TPrecision rot = rotation[rotationAxis] ;
            rot += dx;
            int r = (int)(rot/360);
            rotation[rotationAxis]= rot - r*360;
          }

          break;
      }
      //}
      last_x = x;
      last_y = y; 
      glutPostRedisplay();
    };

    virtual void idle(){};


    virtual void init(){  };





};

#endif
