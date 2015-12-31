#ifndef HYPERBOLICORTHOGONALPROJECTOR_H
#define HYPERBOLICORTHOGONALPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class HyperbolicOrthogonalProjector : public AbstractPlanarProjector<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;



  public:


    HyperbolicOrthogonalProjector(Data<TPrecision> &d, bool scale, bool tri, bool fix) :
      AbstractPlanarProjector<TPrecision>(d, scale, tri, fix) {
    };





    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = this->A * (x - this->center);
      TPrecision n = xp.norm();
      if( n > 10e-10){
        TPrecision d = cosh( n / (this->scale * this->scaleFactor) );        
        xp *= 1.0*sqrt( (d-1 ) / (d+1) ) / n ;
      }
      return xp;
    };


};

#endif
