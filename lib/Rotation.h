#ifndef ROTATION_H
#define ROTATION_H

#include "Eigen/Dense"

#include "Animation.h"
#include "Projector.h"


template<typename TPrecision>
class Rotation : public Animation{

  public:
  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    Projector<TPrecision> *projector;


    VectorXp target;
    VectorXp secant;

    int nSteps;
    int cStep;
    int rIndex;
    

    
  public:
    


    Rotation(Projector<TPrecision> *p=NULL, int t=-1) : projector(p), rIndex(t){ 


      nSteps = 25;
      cStep = nSteps+1;

    };



   void setTarget(const Eigen::Ref< const VectorXp > &t){
     try{
       VectorXp v = projector->getProjector( rIndex );
       target =  t;
       secant = target-v;
       cStep = 0;
     }catch(...){
     }

   };

   bool isRunning(){
    return cStep <= nSteps;
   };


   void step(){
     if( isRunning() ){
       double ratio = cStep/(double)nSteps;
       VectorXp v = target + (ratio - 1.0) * secant;
       projector->animateProjection( v, rIndex );

       /*
       v = v.normalized();
       for(int i=0; i<rIndex; i++){
         TPrecision d = data.getProjector(i).dot( v );
         v -= d*data.getProjector(i);
         v.normalize();
       }

       data.setProjection(v, rIndex);

       for(int i=rIndex+1; i<data.getNumberOfProjectors(); i++){
         TPrecision d = v.dot( data.getProjection(i) );
         VectorXp vi = data.getProjection(i) - d*v;
         vi.normalize();
         data.setProjection(vi, i);
       }

       data.updateProjection();
       */

       cStep++;
       glutPostRedisplay();
     }
   };



};

#endif
