#ifndef OFFSETMATRIXDEL_H
#define OFFSETMATRIXDEL_H

#include "Eigen/Dense"

#include "Display.h"





template<typename TPrecision>
class OffsetMatrixDEL : public AdjustableDisplayElement{

  private:
    typedef typename Eigen::Matrix< TPrecision, Eigen::Dynamic, Eigen::Dynamic > MatrixXp;
    typedef typename Eigen::MatrixXi MatrixXi;
    typedef typename Eigen::Matrix< TPrecision, Eigen::Dynamic, 1 > VectorXp;


#define BUFSIZE 512

    Data<TPrecision> &data;
    
    int m, n;
    int mAll, nAll;
    MatrixXi offsets;
    bool keepAspect;
    bool transpose;
    bool cMajor;
    double alpha; 





  public:

    OffsetMatrixDEL( Data<TPrecision> &d, int nRows, int nCols, int nRowsAll,
        int nColsAll, Eigen::Ref<MatrixXi> off, bool columnMajor=true, bool tpose=false, bool
        aspect=true ) : data(d), m(nRows), n(nCols), mAll(nRowsAll),
                        nAll(nColsAll), offsets(off), keepAspect(aspect),
                        transpose(tpose), cMajor(columnMajor){ 
      alpha=1;
    };


    void init(){  
    };


    void displayAction(void){
      std::vector<int> &sel = data.getSelection();
      if(sel.size() == 0){
        return;
      }
      
      double xs = width/ (double)nAll;
      double ys = height/ (double)mAll;
      if(keepAspect){
        if(xs < ys){
          ys = xs;
        }
        else{
          xs = ys;
        }
      }
      
      std::vector<int> &selected = data.getSelection();

      for(int s=0; s<selected.size(); s++){
        VectorXp x = data.getData(selected[s]);

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

        double xOff = offsets(0, selected[s] ) * xs;
        double yOff = offsets(1, selected[s] ) * ys;
        if(transpose){
          image = image.transpose();
          std::swap(xOff, yOff);
        }


        glBegin(GL_QUADS);
        for(int i = 0; i < image.rows(); i++){
          for(int j = 0; j < image.cols(); j++){
            double col = image(i,j); 
            glColor4f( col, col, col, alpha );
            glVertex2f( xOff + xLeft + j*xs, yOff + yTop + i*ys);
            glVertex2f( xOff + xLeft + (j+1)*xs, yOff + yTop + i*ys);
            glVertex2f( xOff + xLeft + (j+1)*xs, yOff + yTop + (i+1)*ys);
            glVertex2f( xOff + xLeft + j*xs, yOff + yTop + (i+1)*ys);
          }
        } 
        glEnd();

      }

    };




      void idle(){
      };



      void keyboard(unsigned char key, int x, int y){
      };



      void special(int key, int x, int y){
        switch(key)
        {
          case GLUT_KEY_LEFT:
            alpha *= 0.95;
            break;
          case GLUT_KEY_RIGHT:
            alpha *= 1.05;
            break;
        }
        if(alpha < 0){
          alpha = 0;
        }
        else if(alpha > 1){
          alpha = 1;
        }

        glutPostRedisplay();
      };



      void mouseAction(int button, int state, int x, int y){
      };


      void motionAction(int x, int y){
      };




      void passive(int x, int y){
      };



    };

#endif
