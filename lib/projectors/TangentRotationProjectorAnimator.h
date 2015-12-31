#ifndef TANGENTROTATIONPROJECTORANIMATOR_H
#define TANGENTROTATIONPROJECTORANIMATOR_H

#include "Animator.h"
#include "ProjectorAnimator.h"
#include "AbstractPlanarProjector.h"
#include "DataInterface.h"

template<typename TPrecision>
class TangentRotationProjectorAnimator : public ProjectorAnimator<TPrecision>{
  
  public:
  
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
  
  private:
   
    Animator &animator;
  
    int tangent1, tangent2;


  public:

    TangentRotationProjectorAnimator(Animator &a, int t1=0, int t2=1):animator(a), tangent1(t1), tangent2(t2){
    
    };

    virtual void animateProjection(Projector<TPrecision> *p, const VectorXp &v, int pIndex){

      if(pIndex==2){
        Data<TPrecision> &data = p->getData();
        TPrecision scale = p->getScale(); 
        TPrecision scale2 = scale*scale;

        std::vector< VectorXp > local;
        for(int i=0; i< data.getNumberOfPoints(); i++){
          TPrecision d = ( data.getData(i) - v).squaredNorm() ;
          if(d < scale2){
            local.push_back( data.getData(i) - v );
          }
        }
        int maxDim = std::max(tangent1, tangent2)+5; 
        if(local.size() > maxDim ){
          MatrixXp X(v.size(), local.size() );
          for(int i=0; i< local.size(); i++){
            X.col(i) = local[i];
          }
          EigenLinalg::RandomSVD<TPrecision> svd = 
            EigenLinalg::RandomSVD<TPrecision>(X, maxDim, 2);
          VectorXp t1 =  svd.GetU().col(tangent1); 
          VectorXp t2 =  svd.GetU().col(tangent2);
          
          AbstractPlanarProjector<TPrecision> *ap = dynamic_cast< AbstractPlanarProjector<TPrecision>* >(p);
          if(ap != NULL){
            MatrixXp &A = ap->getPlane();
            if( A.row(0) * t1 < 0 ){
              t1 *= -1;
            }
            if( A.row(1) * t2 < 0 ){
              t2 *= -1;
            }
          } 
          t1 = v + t1*scale;
          t2 = v + t2*scale;

          Rotation<TPrecision> *r1 = new Rotation<TPrecision>(p, 0);
          r1->setTarget(t1);
          animator.addAnimation(r1);
          
          Rotation<TPrecision> *r2 = new Rotation<TPrecision>(p, 1);
          r2->setTarget(t2);
          animator.addAnimation(r2);

        }
      }

      Rotation<TPrecision> *rotate = new Rotation<TPrecision>(p, pIndex);
      rotate->setTarget(v);
      animator.addAnimation(rotate);
    
    };

};
  
#endif
