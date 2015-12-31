#ifndef PROJECTORBALLSHADOWBACKGROUND_H
#define PROJECTORBALLSHADOWBACKGROUND_H


#include "BallShadowBackground.h"

template< typename TPrecision>
class ProjectorBallShadowBackground : public BallShadowBackground<TPrecision>{
  private:
    int index;
  
  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    ProjectorBallShadowBackground(TPrecision r, int i, int n=200, bool scaled=true) : BallShadowBackground<TPrecision>(r, n, scaled), index(i){
    };
    
  
  protected:

    virtual MatrixXp getProjected(Projector<TPrecision> *proj){
      return proj->getProjectionBallShadow(index, this->radius, this->scaled, this->nPoints);
    };

};

#endif
