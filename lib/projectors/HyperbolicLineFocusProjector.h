#ifndef HYPERBOLICLINEFOCUSPROJECTOR_H
#define HYPERBOLICLINEFOCUSPROJECTOR_H

#include "Eigen/Dense"

#include "DataInterface.h"
#include "AbstractLineProjector.h"


template<typename TPrecision>
class HyperbolicLineFocusProjector : public AbstractLineProjector<TPrecision>{

  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


    HyperbolicLineFocusProjector(Data<TPrecision> &d) : AbstractLineProjector<TPrecision>(d) {
    };





    virtual VectorXp project(const VectorXp &x){

      VectorXp p(2);
      p(0) = ( x-this->primary ).dot( this->dir );
      p(1) = ( this->primary + p(0) * this->dir - x ).norm();
      p /= this->scale;

      double f = (this->scaleFactor < 1) ? this->scaleFactor : 1;

      if(p(0) < 0){
        TPrecision dist = cosh( -p(0) / this->scaleFactor );
        p(0) = -f*sqrt( (dist-1)/(dist+1) );
      }
      else if(p(0) > 1){
        TPrecision dist = cosh( (p(0)-1) / this->scaleFactor );
        p(0) = 1 + f*sqrt( (dist-1)/(dist+1) ) ;
      }

      TPrecision dist = cosh( p(1) / this->scaleFactor  );
      p(1) = 2*sqrt( (dist-1)/(dist+1) );
     
      double l = (1.0+2*f);

      p(0) += f;
      p(0) *= 2.0/l;
      p(0) -= 1.0;
      
      p(1) -= 1.0;
      
      return p;
    };
    


};

#endif
