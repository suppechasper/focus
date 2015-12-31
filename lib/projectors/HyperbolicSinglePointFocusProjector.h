#ifndef HYPERBOLICSINGLEPOINTFOCUSPROJECTOR_H
#define HYPERBOLICSINGLEPOINTFOCUSPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class HyperbolicSinglePointFocusProjector : public AbstractPlanarProjector<TPrecision>{


  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


  public:


    HyperbolicSinglePointFocusProjector(Data<TPrecision> &d, bool scale, bool tri, bool fix):
      AbstractPlanarProjector<TPrecision>(d, scale, tri, fix){
    };





    virtual VectorXp project(const VectorXp &x){
      VectorXp v = x-this->center;
      TPrecision d = v.norm();
      VectorXp vp = this->A * v;
      TPrecision vn = vp.norm();
      if(vn < 10e-10){
       vp = VectorXp::Zero(2);
       vp(0)=1;
      }
      else{
        vp /= vn;
      }      
      d = cosh(  d / ( this->scale * this->scaleFactor)  );
      vp *= sqrt( (d-1 ) / (d+1) ) ;

      return vp;
    };


};

#endif
