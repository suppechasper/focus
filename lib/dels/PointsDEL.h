#ifndef PointsDEL_H
#define PointsDEL_H

#include "Display.h"
#include "Eigen/Dense"
#include "Rotation.h"
#include "Animator.h"
#include "Font.h"
#include "Projector.h"


template<typename TPrecision>
class PointsDEL : public AdjustableDisplayElement{

  private:

    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


    Data<TPrecision> &data;
    Font &font;

    std::vector<MatrixXp> coords;
   

  public:
    PointsDEL(Data<TPrecision> &d, Font &f) : data( d ), font(f){ 
      coords = std::vector<MatrixXp>( d.getNumberOfPoints() );
    };



    ~PointsDEL(){
    };


    void setPoints(int id, const Eigen::Ref<const MatrixXp> x){
      coords[id] = x;
    };

    void init(){  

    };


    void displayAction(void){

      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();

      if(this->drawBackground() ){

        glColor4f(0.25, 0.25, 0.25, 1);
        glPointSize(0.1);
        glBegin(GL_POINTS);

        for(int i=0; i<coords.size(); i++){
          MatrixXp &X = coords[i];
          for(int j =0; j <X.cols(); j++){
            glVertex2f(xLeft + X(0, j)*width, yTop + height* X(1, j) );
          }
        }
        glEnd();
      }



      std::vector<int> &sel =data.getSelection();   

      glBegin(GL_POINTS);
      for(int i=0; i<sel.size(); i++){
        RGB &col = data.getColor(sel[i]);

        glColor4f(col.r, col.g, col.b, 1);
        MatrixXp &X = coords[sel[i]];
        glPointSize(2);
        for(int j =0; j <X.cols(); j++){
          glVertex2f(xLeft + X(0, j)*width, yTop + height*X(1, j) );
        }
      }

      glEnd();

    };


    void idle(){

    };


    void keyboard(unsigned char key, int x, int y){
    };



    void special(int key, int x, int y){    
    };




    void mouseAction(int button, int state, int x, int y){
    };



    void motionAction(int x, int y){
    };


    void passive(int x, int y){
    };









    



};

#endif








