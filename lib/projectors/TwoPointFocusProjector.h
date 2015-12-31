#ifndef TWOPOINTFOCUSPROJECTOR_H
#define TWOPOINTFOCUSPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class TwoPointFocusProjector : public AbstractPlanarProjector<TPrecision>{


  private:

    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    MatrixXp P;


  public:


    TwoPointFocusProjector(Data<TPrecision> &d, bool scale) :
      AbstractPlanarProjector<TPrecision>(d, scale, true, true) {
    };


    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = VectorXp::Zero(2);
      xp(0) = (x-this->projector[0]).norm() / this->scale;
      xp(1) = (x-this->projector[1]).norm() / this->scale;
      return xp;
    };



  protected:
    
    virtual void update(unsigned int rIndex){
      if( rIndex<2){   
        this->center = (this->projector[0] + this->projector[1]) / 2.0;      
        AbstractPlanarProjector<TPrecision>::update(rIndex);
      }

    };



};

#endif
