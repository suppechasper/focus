#ifndef ANIMATIONPROJECTOROBSERVER_H
#define ANIMATIONPROJECTOROBSERVER_H


#include "Rotation.h"
#include "ProjectorObserver.h"

template<typename TPrecision>
class AnimationProjectorObserver : public ProjectorObserver<TPrecision> {


  private:
    Animator &animator;

  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    
    AnimationProjectorObserver(Animator &a) : animator(a){
    };

    virtual void projectionChanged(Projector<TPrecision> *proj, const VectorXp &x, int pIndex) {
      Rotation<TPrecision> rotate(proj, pIndex);
      animator.setAnimation(rotate);
    };

};





#endif
