#ifndef PROJECTORANIMATOR_H
#define PROJECTORANIMATOR_H

template<typename TPrecision> class Projector;

template<typename TPrecision>
class ProjectorAnimator{
  
  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    
    virtual ~ProjectorAnimator() {};
    virtual void animateProjection(Projector<TPrecision> *p, const VectorXp &v, int i){
      p->animateProjection(v, i);
    };

};
  
#endif
