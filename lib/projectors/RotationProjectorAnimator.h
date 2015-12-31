#ifndef ROTATIONPROJECTORANIMATOR_H
#define ROTATIONPROJECTORANIMATOR_H

#include "Animator.h"
#include "ProjectorAnimator.h"

template<typename TPrecision>
class RotationProjectorAnimator : public ProjectorAnimator<TPrecision>{
  
  public:
  
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
  
  private:
  
    Animator &animator;
  
  public:
    RotationProjectorAnimator(Animator &a):animator(a){
    
    };

    virtual void animateProjection(Projector<TPrecision> *p, const VectorXp &v, int i){
      Rotation<TPrecision> *rotate = new Rotation<TPrecision>(p, i);
      rotate->setTarget(v);
      animator.addAnimation(rotate);
    };

};
  
#endif
