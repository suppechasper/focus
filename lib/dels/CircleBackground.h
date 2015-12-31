#ifndef CIRCLEBACKGROUND_H
#define CIRCLEBACKGROUND_H


#include "ProjectionBackground.h"

template< typename TPrecision>
class CircleBackground : public ProjectionBackground<TPrecision>{

  protected:
    TPrecision radius;
    int n;

  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    CircleBackground(TPrecision r, int nP = 200) : radius(r), n(nP) {
    };




    void drawBackground( ProjectionDEL<TPrecision> *del, Projector<TPrecision> *proj ){
      glLineWidth(1);

      glColor4f(this->color.r, this->color.g, this->color.b, 1);

      glBegin(GL_LINE_LOOP);
      for(int i =0; i <= n; i++){
        double angle = 2 * M_PI * i / n;
        VectorXp x = del->toScreen( cos(angle)*radius, sin(angle)*radius );
        glVertex2d( x(0), x(1) );
      }
      glEnd();

    };    

};

#endif
