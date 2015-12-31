#ifndef SPHERICALPLANARPROJECTOR_H
#define SPHERICALPLANARPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class SphericalPlanarProjector : public AbstractPlanarProjector<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;






  public:


    SphericalPlanarProjector(Data<Tprecision> &d, bool scale, bool tri, bool fix) :
      AbstractPlanarProjector<TPrecision>(d, scale, tri, fix) {
    };





    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = (x - this->center).normalized() * this->scale;
      return A*xp;
    };

 


};

#endif
