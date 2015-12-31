#ifndef CENTERBALLSHADOWBACKGROUND_H
#define CENTERBALLSHADOWBACKGROUND_H


#include "BallShadowBackground.h"

template< typename TPrecision>
class CenterBallShadowBackground : public BallShadowBackground<TPrecision>{
  
  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    CenterBallShadowBackground(TPrecision r, int n=200, bool scaled=true) : BallShadowBackground<TPrecision>(r, n , scaled){
    };
    



  
  protected:

    virtual MatrixXp getProjected(Projector<TPrecision> *proj){
      return proj->getProjectionBallShadow( proj->getCenter(), this->radius, this->scaled, this->nPoints );
    };

};

#endif
