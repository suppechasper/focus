#ifndef LINEARPROJECTOR_H
#define LINEARPROJECTOR_H

#include "Eigen/Dense"
#include "AbstractPlanarProjector.h"


template<typename TPrecision>
class LinearProjector : public AbstractPlanarProjector<TPrecision>{


  private:

    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    MatrixXp P;


  public:


    LinearProjector(Data<TPrecision> &d, bool scale, bool fix) :
      AbstractPlanarProjector<TPrecision>(d, scale, true, fix) {
    };


    virtual VectorXp project(const VectorXp &x){
      VectorXp xp = P * (x - this->center);
      xp /= this->scale;
      return xp;
    };



  protected:
    
    virtual void update(unsigned int rIndex){
      AbstractPlanarProjector<TPrecision>::update(rIndex);

      if(P.cols() == 0){
        P = MatrixXp::Zero(2, this->getDimension() );
      }
      if( rIndex<2){   
        P.row(rIndex) = (this->projector[rIndex] - this->center).normalized();
      }
      
    };



};

#endif
