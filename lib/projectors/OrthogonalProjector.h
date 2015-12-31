#ifndef ORTHOGONALPROJECTOR_H
#define ORTHOGONALPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class OrthogonalProjector : public AbstractPlanarProjector<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;






  public:


    OrthogonalProjector(Data<TPrecision> &d, bool scale, bool tri, bool fix) :
      AbstractPlanarProjector<TPrecision>(d, scale, tri, fix) {
    };





    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = this->A * (x - this->center);
      xp /= this->scale * this->scaleFactor;
      return xp;
    };

 


};

#endif
