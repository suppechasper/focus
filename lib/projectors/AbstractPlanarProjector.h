#ifndef ABSTRACTPLANARPROJECTOR_H
#define ABSTRACTPLANARPROJECTOR_H

#include "Eigen/Dense"
#include "Projector.h"


template<typename TPrecision>
class AbstractPlanarProjector : public Projector<TPrecision>{

  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

  private:
    bool scaled;
    bool tripod;
    bool fixed;

  protected:
    MatrixXp A; 
    VectorXp center;
    TPrecision scale;


  public:


    AbstractPlanarProjector(Data<TPrecision> &d, bool doScale, bool tri, bool fix) :
      Projector<TPrecision>(d), scaled(doScale), tripod(tri), fixed(fix){
      
      int dim = d.getDimension();

      scale = 1;
      A = MatrixXp::Zero( 2, dim );
      center = VectorXp::Zero(dim);
    
      for(int i=0; i<2; i++){
        VectorXp v = VectorXp::Zero(dim);
        v(i) = 1;
        this->addProjector(v);
      }
      //if(!fixed){
        this->addProjector( center ); 
      //}

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
      if(scaled){
        r *= this->scale;
      }
      for(int i =0; i < n; i++){
        double angle = (2.0 * M_PI * i) / n;
        X.col(i) = this->project( x + r * (  A.row(0).transpose() * cos(angle) +
                                             A.row(1).transpose() * sin(angle) ) );
      }

      return X;

    };



    MatrixXp &getPlane(){
      return A;
    };

  protected:
  

    virtual void update(unsigned int rIndex){

      if( rIndex<2){ 

        VectorXp v = this->projector[rIndex];
        if(!fixed){
         v -= center;
        }
        for(unsigned int i=0; i<rIndex; i++){
          TPrecision d = A.row(i).dot( v );
          v -= d * A.row(i);
        }
        A.row(rIndex) = v.normalized();

        if(rIndex  ==   0 && this->projector.size() > 1){
          //VectorXp p =A.row(rIndex+1);
          AbstractPlanarProjector::update( rIndex+1); 
        }
      }
      else if(rIndex == 2 && !fixed && this->projector.size() > 2){
        if(tripod){
          center = this->projector[2];
          AbstractPlanarProjector::update(0);
        }
        else{
          //IRK: changes projectors without notfication
          VectorXp off = this->projector[2] - center;
          this->projector[0] += off;
          this->projector[1] += off;
          
          center = this->projector[2];
        }
      }

      if( scaled && this->projector.size() > 1){
        scale = std::max( (this->projector[0] - center).norm(),  (this->projector[1] - center).norm() );     
      }
    };


};

#endif
