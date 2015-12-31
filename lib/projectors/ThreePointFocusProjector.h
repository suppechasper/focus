#ifndef THREEPOINTFOCUSPROJECTOR_H
#define THREEPOINTFOCUSPROJECTOR_H

#include "Eigen/Dense"
#include "Projector.h"

#include <vector>


template<typename TPrecision>
class ThreePointFocusProjector : public Projector<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


    std::vector<VectorXp> pa;
    std::vector<VectorXp> na;
    std::vector<TPrecision> nal;
    MatrixXp P;
    TPrecision scale;
    VectorXp center;
    VectorXp cp;



  public:


    ThreePointFocusProjector(Data<TPrecision> &d) : Projector<TPrecision>(d)  { 

      int dim = d.getDimension();

      P = MatrixXp(2, dim);

      for(int i=0; i<3; i++){
        VectorXp v = VectorXp::Zero(dim);
        v(i) = 1;
        this->addProjector(v);
      }
    };



    virtual VectorXp project(const VectorXp &x){

      int minIndex = 0;
      TPrecision dist = std::numeric_limits<TPrecision>::max();
      VectorXp v;
      for(int i=0; i<3; i++){
        VectorXp vTmp = (x - this->projector[i]);
        TPrecision tmp = vTmp.norm();
        if( tmp < dist ){
          minIndex = i;
          v = vTmp;
          dist = tmp;
      
        }
      }
      return project(v, minIndex, dist);
    };
    
    

    virtual VectorXp project(const VectorXp &x, int index ){
      VectorXp v = this->projector[index] - x;
      return project(v, index, v.norm() );  
    };


    virtual VectorXp project(const VectorXp &v, int index, TPrecision dist ){

      if(index > 3 || index < 0){
        throw "Only 3 projectors";
      }

      VectorXp vp = P*v;

      TPrecision vn = vp.norm();
      if(vn < 10e-10){
       vp = VectorXp::Zero(2);
       vp(0)=1;
      }
      else{
        vp /= vn;
      }

      TPrecision r = std::numeric_limits<TPrecision>::max();
      for(int i=0; i<3; i++){
        if(i != index){
          TPrecision tmp = fabs( nal[i] / vp.dot(na[i]) );
          if( tmp  < r ){
            r = tmp;
          }
        }
      }
      dist = cosh( dist  / (r*this->scaleFactor)  );
      vp *= r*sqrt( (dist-1 ) / (dist+1) ) ;

      return (vp + pa[index]-cp)/(2*scale);
    };






    VectorXp getCenter(){
      return center;
    };

    TPrecision getScale(){
      return scale;
    };








    virtual MatrixXp getProjectionBallShadow( const VectorXp &center, TPrecision
        r, bool scaled=true, int n=100 ){
      
      MatrixXp X(2, n);
      if(scaled){
        r *= scale;
      }

      for(int i =0; i < n; i++){
        double angle = (2.0 * M_PI * i) / n;

        X.col(i) = project( center + cos(angle)*r*P.row(0).transpose() + 
            sin(angle)*r*P.row(1).transpose() ); 
      }

      return X;
      
    };
    
   


    virtual MatrixXp getProjectionBallShadow( unsigned int pIndex, TPrecision
        r, bool scaled=true, int n=100 ){

      if(pIndex > 2){
        MatrixXp X(0,0);
        return X;
      };

      MatrixXp X(2, n);
      if(scaled){
        r *= scale;
      }
      for(int i =0; i < n; i++){
        double angle = (2.0 * M_PI * i) / n;
        X.col(i) = project( cos(angle)*r*P.row(0).transpose() + 
            sin(angle)*r*P.row(1).transpose() , pIndex, r); 
      }
      return X;

    }; 




  protected:
    
    void update(unsigned int rIndex){

      if(this->projector.size() < 3){
        return;
      };
      pa.resize(3);
      na.resize(3);
      nal.resize(3);

      na[0] = this->projector[2]-this->projector[1];
      na[1] = this->projector[0]-this->projector[2];
      na[2] = this->projector[1]-this->projector[0];
      for(int i=0; i<3; i++){
        nal[i] = na[i].norm();
        na[i] /= nal[i];
        nal[i] *= 0.5;
      };
      

      P.row(0) = na[0];

      TPrecision d = na[1].dot( na[0] );
      P.row(1) = ( na[1] - d * na[0] ).normalized();
     
       
      for(int i=0; i<3; i++){
        na[i] = P * na[i];
        pa[i] = P * this->projector[i];
      }
      
      center = VectorXp::Zero( this->getDimension() );
      for(int i=0; i<3; i++){
        center += this->projector[i];
      }
      center /= 3;
      cp = P * center;

      scale = 0;
      for(int i=0; i<3; i++){  
        scale = std::max(scale, (cp - pa[i]).norm() );
      }

    };
    

};

#endif
