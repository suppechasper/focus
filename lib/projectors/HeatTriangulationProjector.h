#ifndef HEATTRIANGULATIONPROJECTOR_H
#define HEATTRIANGULATIONPROJECTOR_H

#include <vector>


#include "AbstractPlanarProjector.h"



template<typename TPrecision>
class HeatTriangulationProjector: public AbstractPlanarProjector<TPrecision>{
  
  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    

  private:
    

    MatrixXp hA;
    VectorXp mCenter;

    TPrecision scale1, scale2, scaleSquared;
    TPrecision time;
    int hd;



  public:

    HeatTriangulationProjector(Data<TPrecision> &d) : AbstractPlanarProjector<TPrecision>(d, true, true, false){
      scale1 = 1;
      scale2 = 1;
      time = 1;     
      hd = 2;
    };

    virtual ~HeatTriangulationProjector(){      
      

    };



    virtual VectorXp project(const VectorXp &x){
      VectorXp v1 = x- this->center;
      VectorXp v2 = x- this->mCenter;
      TPrecision v1n = v1.squaredNorm();
      TPrecision v2n = v2.squaredNorm();
      VectorXp v = (v1n < v2n) ? v1 : v2;
      

       
      VectorXp px(hd);
      double a1 = (this->scale * this->A.row(0).transpose() - v).squaredNorm(); 
      double a2 = (this->scale * this->A.row(1).transpose() - v).squaredNorm(); 
      double a1n = (-this->scale * this->A.row(0).transpose() - v).squaredNorm(); 
      double a2n = (-this->scale * this->A.row(1).transpose() - v).squaredNorm(); 
      double n = 4.0 * scaleSquared * this->scaleFactor; 
     
      px(0) = exp( -a1 / n )-1; 
      px(1) = exp( -a2 / n )-1; 
      px(2) = exp( -a1n / n )-1; 
      px(3) = exp( -a2n / n )-1; 
      if(v1n > v2n){
        px = -px;
      }
      
      return px;
    };





  

  protected:

    virtual void update( unsigned int i){

       AbstractPlanarProjector<TPrecision>::update(i);
      
       if( this->projector.size() > 2){
         scale1 = (this->projector[0] - this->center).norm();
         scale2 = (this->projector[1] - this->center).norm();    
         scaleSquared = this->scale*this->scale;
         mCenter = this->center + this->scale * ( this->A.row(0).transpose() + this->A.row(1).transpose() ); 
       } 

    };
 
 
 


};





#endif
