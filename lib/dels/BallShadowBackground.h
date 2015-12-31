#ifndef BALLSHADOWBACKGROUND_H
#define BALLSHADOWBACKGROUND_H


#include "ProjectionBackground.h"

template< typename TPrecision>
class BallShadowBackground : public ProjectionBackground<TPrecision>{
  
  protected:
    TPrecision radius;
    int nPoints;
    bool scaled;
  
  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    BallShadowBackground(TPrecision r, int n=200, bool scale=true) : radius(r), nPoints(n), scaled(scale){
    };
    



    void drawBackground( ProjectionDEL<TPrecision> *del, Projector<TPrecision> *proj ){
      MatrixXp X = getProjected(proj);
      
      glLineWidth(1);
      glColor4f(this->color.r, this->color.g, this->color.b, this->alpha);
      
      glBegin(GL_LINE_LOOP);
      for(int i=0; i<X.cols(); i++){
        VectorXp x = del->toScreen( X(0, i), X(1, i) );
        glVertex2f( x(0), x(1) );
      };
      glEnd();

    };

  
  protected:

    virtual MatrixXp getProjected(Projector<TPrecision> *proj) = 0;

};

#endif
