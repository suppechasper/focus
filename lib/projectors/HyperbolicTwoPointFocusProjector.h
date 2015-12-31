#ifndef HYPERBOLICTWOPOINTFOCUSPROJECTOR_H
#define HYPERBOLICTWOPOINTFOCUSPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class HyperbolicTwoPointFocusProjector : public AbstractPlanarProjector<TPrecision>{


  private:

    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    MatrixXp P;





  public:


    HyperbolicTwoPointFocusProjector(Data<TPrecision> &d, bool scale) :
      AbstractPlanarProjector<TPrecision>(d, scale, true, true) {
      update(1);
    };


    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = VectorXp::Zero(2);
      TPrecision d = cosh(  (x-this->projector[0]).norm() / ( this->scale * this->scaleFactor) ); 
      xp(0) = 2*sqrt( (d-1)/(d+1) )-1;
      d = cosh( (x-this->projector[1]).norm() / (this->scale* this->scaleFactor)   ); 
      xp(1) = 2*sqrt( (d-1)/(d+1) )-1;
      return xp;
    };






  protected:
    

    virtual void update(unsigned int rIndex){
      
      if( rIndex < 2 && this->projector.size() > 1){   
        this->center = (this->projector[0] + this->projector[1]) / 2.0;      
      }
      AbstractPlanarProjector<TPrecision>::update(rIndex);

    };
    



};

#endif
