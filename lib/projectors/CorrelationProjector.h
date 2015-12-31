#ifndef CORRELATIONPROJECTOR_H
#define CORRELATIONPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class CorrelationProjector : public AbstractPlanarProjector<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;






  public:


    CorrelationProjector(Data<TPrecision> &d) :
      AbstractPlanarProjector<TPrecision>(d, false, true, true) {
    };

    void animateProjection(const VectorXp &v, unsigned int i){
        this->projector[i] = v.array() - v.mean();
        this->update(i);
        this->updateProjected();
    };


    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = x.array() - x.mean() ;
      return this->A * xp.normalized();
    };

 


};

#endif
