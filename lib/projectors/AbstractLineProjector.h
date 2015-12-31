#ifndef ABSTRACTLINEPROJECTOR_H
#define ABSTRACTLINEPROJECTOR_H

#include "Eigen/Dense"

#include "Projector.h"


template<typename TPrecision>
class AbstractLineProjector : public Projector<TPrecision>{

  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

  protected:
    VectorXp primary;
    VectorXp secondary;
    VectorXp center;
    VectorXp dir;
    VectorXp normal;
    TPrecision scale;


  public:


    AbstractLineProjector(Data<TPrecision> &d) : Projector<TPrecision>(d) {

      this->addProjector( VectorXp::Zero( d.getDimension() ) ); 
      this->addProjector( VectorXp::Ones( d.getDimension() ) );
    };


    VectorXp getCenter(){
      return center;
    };

    TPrecision getScale(){
      return scale;
    };

    MatrixXp getProjectionBallShadow(const VectorXp &x, TPrecision r, bool
        scaled=true, int n=200){ 

      MatrixXp X(2, n);

      r *= scale;
      for(int i =0; i < n; i++){
        double angle = (2.0 * M_PI * i) / n;
        X.col(i) = this->project( x + r * (dir*cos(angle) + normal*sin(angle) ) );
      }

      return X;

    };


  private:
    
    void update(unsigned int rIndex){ 
      if(this->projector.size() < 2){
        return;
      };

      primary = this->projector[0];
      secondary = this->projector[1];
      dir = (secondary-primary);
      scale = dir.norm();
      dir/=scale;
      //scale *= 2.0/3.0;
      
      center = (secondary+primary) / 2.0;

      normal = VectorXp::Constant( this->getDimension(), 1.0/sqrt(dir.size()) );
      TPrecision d = normal.dot(dir);
      if(d > 1.0 - 10e-10){
        normal(0) *= -1;
        d = normal.dot(dir);
      }
      normal = (normal - d*dir).normalized();
      
    };

};

#endif
