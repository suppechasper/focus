#ifndef HORIZONTALLINESCIRCLEBACKGROUND_H
#define HORIZONTALLINESCIRCLEBACKGROUND_H


#include "ProjectionBackground.h"

template< typename TPrecision>
class HorizontalLinesCircleBackground : public ProjectionBackground<TPrecision>{
  
  protected:
    TPrecision radius;
    int nHorizontal;

  
  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    HorizontalLinesCircleBackground(TPrecision r, int n) : radius(r), nHorizontal(n) {
    };
    



    void drawBackground( ProjectionDEL<TPrecision> *del, Projector<TPrecision> *proj ){
      glLineWidth(1);
      glColor4f(this->color.r, this->color.g, this->color.b, this->alpha);

      glBegin(GL_LINES);

      for(int i=0; i < nHorizontal; i++){
        double angle = acos( 1.0*i / nHorizontal  );
        VectorXp x = radius * del->toScreen( cos(angle), sin(angle) );
        VectorXp nx = radius * del->toScreen( -cos(angle), -sin(angle) );

        glVertex2d(  x(0) ,   x(1) );
        glVertex2d( nx(0) ,   x(1) );
        
        glVertex2d(  x(0) ,  nx(1) );
        glVertex2d( nx(0) ,  nx(1) );
      
      }
      
      glEnd();

    };

};

#endif
