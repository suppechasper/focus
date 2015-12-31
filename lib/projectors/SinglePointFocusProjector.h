#ifndef SINGLEPOINTFOCUSPROJECTOR_H
#define SINGLEPOINTFOCUSPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class SinglePointFocusProjector : public AbstractPlanarProjector<TPrecision>{


  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


  public:


    SinglePointFocusProjector(Data<TPrecision> &d, bool scale, bool tri, bool fix):
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
      vp *= d/this->scale * this->scaleFactor;

      return vp;
    };


};

#endif
