#ifndef MATRIXDEL_H
#define MATRIXDEL_H

#include "Eigen/Dense"

#include "Display.h"





template<typename TPrecision>
class MatrixDEL : public AdjustableDisplayElement{

  private:
    typedef typename Eigen::Matrix< TPrecision, Eigen::Dynamic, Eigen::Dynamic > MatrixXp;
    typedef typename Eigen::Matrix< TPrecision, Eigen::Dynamic, 1 > VectorXp;


#define BUFSIZE 512

    Data<TPrecision> &data;
    
    int m, n;
    bool keepAspect;
    bool transpose;
    bool cMajor;
   





  public:

    MatrixDEL( Data<TPrecision> &d, int nRows, int nCols, bool columnMajor=true,
              bool tpose=false, bool aspect=true ) : data(d), m(nRows),
              n(nCols), keepAspect(aspect), transpose(tpose),
              cMajor(columnMajor){ 
      
    };


    void init(){  
    };


    void displayAction(void){
      std::vector<int> &sel = data.getSelection();
      if(sel.size() == 0){
        return;
      }
      
      double xs = width/ (double)n;
      double ys = height/ (double)m;
      if(keepAspect){
        if(xs < ys){
          ys = xs;
        }
        else{
          xs = ys;
        }
      }
      
      VectorXp x = data.getSelectionMean();


      MatrixXp image = MatrixXp::Zero(m, n);
      if(cMajor){
        int index = 0;
        for(int i=0; i<image.cols(); i++){
          for(int j=0; j<image.rows(); j++){
            image(j, i) = x(index);
            ++index; 
          }
        }

      }
      else{        
        int index = 0;
        for(int i=0; i<image.rows(); i++){
          for(int j=0; j<image.cols(); j++){
            image(i, j) = x(index);
            ++index;
            if(index == x.size() ){
              break;
            }
          }
        }
      }
      image.array() -= image.minCoeff();
      image /= image.maxCoeff();

      if(transpose){
        image = image.transpose();
      };

      glBegin(GL_QUADS);
      for(int i = 0; i < image.rows(); i++){
        for(int j = 0; j < image.cols(); j++){
          double col = image(i,j); 
          glColor3f( col, col, col );
          glVertex2f( xLeft + j*xs, yTop + i*ys);
          glVertex2f( xLeft + (j+1)*xs, yTop + i*ys);
          glVertex2f( xLeft + (j+1)*xs, yTop + (i+1)*ys);
          glVertex2f( xLeft + j*xs, yTop + (i+1)*ys);
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
