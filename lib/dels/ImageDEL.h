#ifndef IMAGEDEL_H
#define IMAGEDEL_H

#include "Eigen/Dense"

#include "Display.h"
#include "Animator.h"

#include "GMRAData.h"

class PointIndexConverter{
  public:
    virtual std::vector<Eigen::VectorXi> convertIndex(int index) = 0;
};




class GeneralPurposeConverter : public PointIndexConverter{
  private:
      Eigen::MatrixXi conv;


  public:
    
    GeneralPurposeConverter(Eigen::Ref<Eigen::MatrixXi> c) : conv(c) {
    };

    std::vector<Eigen::VectorXi> convertIndex(int index){
      std::vector< Eigen::VectorXi > ind(1);
      ind[0] = conv.col(index) ;
      return ind;
    };
};




class MultiSetGeneralPurposeConverter : public PointIndexConverter{
  
  private:

      Eigen::MatrixXi conv;
      std::vector< std::vector<int> > multiIndex;



  public:


    MultiSetGeneralPurposeConverter(Eigen::Ref<Eigen::MatrixXi> c, std::vector<
        std::vector<int> > &mIndex) : conv(c), multiIndex(mIndex) { };


    std::vector< Eigen::VectorXi > convertIndex( int index ){

      std::vector<int> &m = multiIndex[index];
      std::vector< Eigen::VectorXi > ind( m.size() );
      for(unsigned int i=0; i<m.size(); i++){
        ind[i] = conv.col( m[i] );
      }
      return ind;

    };


};


template<typename TPrecision>
class GMRAConverter : public PointIndexConverter{
  
  private:

      Eigen::MatrixXi conv;
      GMRAData<TPrecision> *data;

  public:
    
    GMRAConverter(Eigen::Ref<Eigen::MatrixXi> c, GMRAData<TPrecision> *d) : conv(c), data(d) { };




    std::vector< Eigen::VectorXi > convertIndex( int index ){

      std::vector<int> m = data->getNodePoints( index );
      std::vector< Eigen::VectorXi > ind( m.size() );
      for(int i=0; i<m.size(); i++){
        ind[i] = conv.col( m[i] );
      }
      return ind;

    };


};









template<typename TPrecision>
class ImageDEL : public AdjustableDisplayElement{

  private:
    typedef typename Eigen::Matrix< TPrecision, Eigen::Dynamic, Eigen::Dynamic > MatrixXp;
    typedef typename Eigen::Matrix< TPrecision, Eigen::Dynamic, 1 > VectorXp;


#define BUFSIZE 512

    Data<TPrecision> &data;
    MatrixXp image;
    PointIndexConverter *conv;
    
    int xM, yM;
    int mod, cur_button; 
    int pickH, pickW;  

    bool keepAspect;
   

    double alpha;




  public:

    ImageDEL( Data<TPrecision> &d, Eigen::Ref<MatrixXp> im, PointIndexConverter
        *pic ) : data(d), image(im), conv(pic) { 
      pickW = 2;
      pickH = 2;
      xM = -1;
      yM = -1;
      keepAspect=true;

      image.array() -= image.minCoeff();
      image.array() /= image.maxCoeff();

      alpha = 0.2;
    };


    void init(){  
    };


    void displayAction(void){
      double xs = width/ (double)image.cols();
      double ys = height/ (double)image.rows();
      if(keepAspect){
        if(xs < ys){
          ys = xs;
        }
        else{
          xs = ys;
        }
      }

      if(this->drawBackground() ){
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
      }

      //highlight selected
      std::vector<int> &sel = data.getSelection();
      for( int k = 0; k < sel.size(); k++ ){
          
        
        glBegin(GL_QUADS);
        if(data.isActive(sel[k])){
          std::vector< Eigen::VectorXi > ps = conv->convertIndex( sel[k] );
          RGB &col=data.getColor( sel[k] );
          //if(k==0){
          //  glColor4f( col.r, col.g, col.b, 1 );
          // }
          //else{
          glColor4f( col.r, col.g, col.b, alpha );
          //}   
          for(int l=0; l<ps.size(); l++){
            Eigen::VectorXi p = ps[l];
            int i=p(0);
            int j=p(1);

            glVertex2f( xLeft + j*xs, yTop + i*ys);
            glVertex2f( xLeft + (j+1)*xs, yTop + i*ys);
            glVertex2f( xLeft + (j+1)*xs, yTop + (i+1)*ys);
            glVertex2f( xLeft + j*xs, yTop + (i+1)*ys);
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
        alpha=0;
      }
      else if(alpha > 1){
        alpha=1;
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
