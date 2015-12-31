#ifndef PCADEL_H
#define PCADEL_H

#include "AdjustableDisplayElement.h"
#include "EigenRandomSVD.h"
#include "Eigen/Dense"
#include "Rotation.h"
#include "Animator.h"

#include <algorithm>

template<typename TPrecision>
class PCADEL : public AdjustableDisplayElement{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


#define BUFSIZE 512

    SVDObserver<TPrecision> *svd;

    Projector<TPrecision> &projector;
    RGB col;
    
    int nTop;
    
    int groupID;
    int lw;
    int selected;

   


  public:

    PCADEL(SVDObserver<TPrecision> *obs, Projector<TPrecision> &p, const RGB &color)   : svd(obs),
      projector(p), col(color){ 
      
      lw = 4;
      selected = -1;
      nTop=10;

   };





    void init(){  

    };


    void displayAction(void){

      if(svd->getS().size() == 0){
        return;
      }
      VectorXp v = svd->getS().array().square();
      TPrecision ma = v.maxCoeff();
      TPrecision mi = v.minCoeff();
      v = v.array()-mi;
      v = v/(ma-mi);


     
      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
     
      
      glPushName(-1); 
      glLineWidth(lw);
      for(int i=0; i < std::min(nTop, (int)v.size()); i++){
        if(i==selected){
          glColor4f(col.r, col.g, col.b, 1);
        }
        else{
          glColor4f(col.r, col.g, col.b, 0.75);
        }
        glLoadName(i);
        glBegin(GL_LINES);
        glVertex2f(xLeft+(nTop-i)*(lw+1), yTop + height * (1.0 - v(i)) );
        glVertex2f(xLeft+(nTop-i)*(lw+1), yTop + height );
        glEnd();
      }




      glLoadName(-1); 




      glPopName();

    };


    void idle(){

    };


    void keyboard(unsigned char key, int x, int y){
    };



    void special(int key, int x, int y){
    };



    void mouseAction(int button, int state, int x, int y){
      if(!isInside(x, y)){ return; };

      if (state == GLUT_DOWN && selected >= 0){
        if(button == GLUT_LEFT_BUTTON ){
          projector.setProjector( svd->getU().col(selected), 0 );
        }
        else if(button == GLUT_MIDDLE_BUTTON){
          projector.setProjector( svd->getU().col(selected), 2 );
        }
        else if(button == GLUT_RIGHT_BUTTON){
          projector.setProjector( svd->getU().col(selected), 1 );
        }

        selected=-1;
      }
    };



    // catch mouse move events
    void motionAction(int x, int y){

    };




    void passive(int x, int y){
      if(!isInside(x, y)){ return; };

      GLint vp[4];
      glGetIntegerv(GL_VIEWPORT, vp);
      GLuint selectBuf[BUFSIZE];
      glSelectBuffer(BUFSIZE, selectBuf);
      glRenderMode(GL_SELECT);
      glInitNames();

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix(x, vp[3]-y, 2, 2, vp);
      glOrtho (0, vp[2], vp[3], 0, 0, 1);

      display();
      GLint hits = glRenderMode(GL_RENDER);

      for(int i=0; i<hits; i++){
        int tmp = selectBuf[i*4 + 3];
        if(tmp != -1){
          selected = tmp;
        }
      }

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glutPostRedisplay();
    };



};

#endif
