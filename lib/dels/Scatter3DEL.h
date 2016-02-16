#ifndef SCATTER3DEL_H
#define SCATTER3DEL_H


#include <vector>

#include <Eigen/Dense>
#include <Eigen/Geometry> 

#include "DataInterface.h"

#include "AdjustableDisplayElement.h"

#include "Font.h"



template<typename TPrecision>
class Scatter3DEL : public AdjustableDisplayElement{

  protected:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    typedef typename Eigen::Vector3d Vector3d;


#define BUFSIZE 512

    Data<TPrecision> &data;

    
    //mouse 
    int last_x;
    int last_y;
    int cur_button;
    int mod;

    //naviagtion
    Vector3d eye;
    Vector3d up;
    Vector3d center;
    float pAngle;

    float alpha;
    float pointSize;



  public:

    Scatter3DEL(Data<TPrecision> &d ) 
      : data( d ) { 

        pointSize = 6;

        mod = 0;
        cur_button = -1;

        //naviagtion
        up = Vector3d::Zero(3); 
        up(1) = 1;
        center = Vector3d::Constant(0.5);
        center(2) = 1;
        eye = Vector3d::Constant(0.5);
        eye(2) = 0; 
        pAngle = 65;
        
    };


    virtual ~Scatter3DEL(){
    };


    virtual void reshape(int wi, int he){
      AdjustableDisplayElement::reshape(wi, he);
    };




    virtual void displayAction(void){

      using namespace Eigen;
  
      glViewport(this->xLeft, this->yTop, this->width, this->height);
      
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      setupPerspective(this->width, this->height); 


      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
      gluLookAt( eye(0), eye(1), eye(2), center(0), center(1), center(2), up(0), up(1), up(2) );

      glPointSize(this->pointSize);
      glBegin(GL_POINTS);
      for(int i=0; i< data.getNumberOfPoints(); i++){
        const VectorXp &p= data.getData(i);


        RGB &col = this->data.getColor(i);
        glColor4f(col.r, col.g, col.b, 0.5 );
        glVertex3f( p(0), p(1), p(2) );

      }
      glEnd();
      
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();

    };





    virtual void keyboard(unsigned char key, int x, int y){
      switch(key){
        case '>':
          pointSize *= 1.05;
          break;
        case '<':
          pointSize /= 1.05;
          break;
        case '+':
          pAngle *= 1.05;
          break;
        case '-':
          pAngle /= 1.05;
          break;
      }
      glutPostRedisplay();
    };



    virtual void special(int key, int x, int y){
      Vector3d dir = center-eye;
      Vector3d o = dir.cross( up );
      switch(key)
      {
        case GLUT_KEY_LEFT:
          eye -= 0.01 * o;
          center -= 0.01 * o;
          break;
        case GLUT_KEY_RIGHT:
          eye += 0.01*o;
          center += 0.01*o;
          break;
        case GLUT_KEY_DOWN:
          eye += 0.01*dir;
          center += 0.01*dir;
          break;
        case GLUT_KEY_UP:
          eye -= 0.01*dir;
          center -= 0.01*dir;
          break;

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
        {
          Vector3d dir = Eigen::AngleAxisd( dx*0.001, up) *  (center-eye);
          center = eye + dir;
          
          Vector3d o = dir.cross( up );
          up = Eigen::AngleAxisd( dy*0.01/pAngle, o) *  up;
          dir = Eigen::AngleAxisd( dy*0.01/pAngle, o) *  dir;
          center = eye + dir;
          break;
        }
        case GLUT_MIDDLE_BUTTON:

          break;

        case GLUT_RIGHT_BUTTON:

          break;
      }
      //}
      last_x = x;
      last_y = y; 
      glutPostRedisplay();
    };

    virtual void idle(){};


    virtual void init(){  };



private:

void setupPerspective(int w, int h){  
  TPrecision sx=1;
  TPrecision sy=1;
  if(w>h){
    sx = (TPrecision)w/h;
  }
  else{
    sy = (TPrecision)h/w;
  }
  gluPerspective(pAngle, ( (float)(w) ) / h, 0.001, 1.5);
  //glOrtho(-4*sx, 4*sx, -4*sy, 4*sy, 16, -16);
}

};

#endif
