#ifndef VERTICALLINESCIRCLEBACKGROUND_H
#define VERTICALLINESCIRCLEBACKGROUND_H


#include "ProjectionBackground.h"

template< typename TPrecision>
class VerticalLinesCircleBackground : public ProjectionBackground<TPrecision>{

  protected:
    TPrecision radius;
    int nVertical;

  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    VerticalLinesCircleBackground(TPrecision r, int nV=0 ) : radius(r), nVertical(nV) {
    };




    void drawBackground( ProjectionDEL<TPrecision> *del, Projector<TPrecision> *proj ){
      glLineWidth(1);

      glColor4f(this->color.r, this->color.g, this->color.b, this->alpha);

      glColor4f(0.2, 0.2, 0.2, 1);
      glBegin(GL_LINES);

      for(int i=0; i < nVertical; i++){
        double angle = acos( 1.0*i / nVertical  );
        VectorXp  x = radius * del->toScreen( cos(angle), sin(angle) );
        VectorXp nx = radius * del->toScreen( -cos(angle), -sin(angle) );

        glVertex2d(  x(0) ,   x(1) );
        glVertex2d(  x(0) ,  nx(1) );

        glVertex2d( nx(0) ,   x(1) );
        glVertex2d( nx(0) ,  nx(1) );

      }

      glEnd();


    };    
};

#endif
