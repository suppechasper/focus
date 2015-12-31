#ifndef LINEPROJECTOR_H
#define LINEPROJECTOR_H

#include "Eigen/Dense"

#include "AbstractLineProjector.h"


template<typename TPrecision>
class LineProjector : public AbstractLineProjector<TPrecision>{



  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


    LineProjector(Data<TPrecision> &d) : AbstractLineProjector<TPrecision>(d) {

    };


    virtual VectorXp project(const VectorXp &x){

      VectorXp p(2);
      p(0) = ( x-this->primary ).dot( this->dir );
      p(1) = ( this->primary + p(0) * this->dir - x ).norm();
      p /= this->scale;

      if(p(0) < -1){
        p(0) = -1 - log( -p(0) );
      }
      else if(p(0) > 3){
        p(0) = 3 + log( p(0)-2 );
      }
      if( p(1) > 3){
        p(1) = 3 + log( p(1)-2 );
      }

      p    *= 2.0/3.0;
      
      p(0) -= 0.3333;
      p(1) -= 1.0;
      return p;
    };


};

#endif
