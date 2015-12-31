#ifndef PROJECTOR_H
#define PROJECTOR_H

#include <vector>

#include "DataInterface.h"

#include "ProjectorAnimator.h"
#include "ProjectorObserver.h"



template<typename TPrecision>
class Projector : public DataObserver<TPrecision>{
  
  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    
  private:
     
    std::vector< ProjectorObserver<TPrecision>* > observers;

    MatrixXp X;
    MatrixXp P;

    ProjectorAnimator<double> *pAnim;

  protected:
    Data<TPrecision> &data;
  
    TPrecision scaleFactor;
    std::vector<VectorXp> projector;



  public:

    Projector(Data<TPrecision> &d) : data(d){
      scaleFactor = 1;
      d.addObserver(this);
      pAnim = new ProjectorAnimator<double>();
    };

    virtual ~Projector(){
      delete pAnim;
    };



    Data<TPrecision> &getData(){
      return data;
    };



    void setProjectorAnimator(ProjectorAnimator<double> *a){
      pAnim = a;
    };

    int getDimension(){
      return data.getDimension();
    };


    MatrixXp &getProjected(){
      return X;
    }



    MatrixXp &getProjectedProjectors(){
      return P;
    };



    void setScaleFactor(TPrecision f){
      if(f >= 0){
        scaleFactor = f;
        updateProjected();
      }
    };



    TPrecision getScaleFactor(){
      return scaleFactor;
    };



    void addObserver(ProjectorObserver<TPrecision> *obs){
      observers.push_back(obs);
    };


    virtual void animateProjection(const VectorXp &v, unsigned int i){
        projector[i] = v;
        update(i);
        updateProjected();
    };

    virtual void setProjector(const VectorXp &v, unsigned int i){
      if(i < projector.size()){
        pAnim->animateProjection(this, v, i);
        this->notifyObservers(v, i);
      }
    };



    virtual VectorXp getProjector(unsigned int i){
      if(i < projector.size() ){
        return projector[i];
      }
      throw "Not that many projectors";
    };



    virtual void addProjector(const VectorXp &v){
      projector.push_back(v);
      update(projector.size()-1);
    };



    virtual int getNumberOfProjectors(){
      return projector.size();
    };


    virtual VectorXp project(const VectorXp &x) = 0;

    virtual VectorXp getCenter() = 0;

    virtual TPrecision getScale() = 0;

    virtual MatrixXp getProjectionBallShadow( const VectorXp &center, TPrecision
        radius, bool scaled=true, int n=100 ) = 0;     


    virtual MatrixXp getProjectionBallShadow( unsigned int pIndex, TPrecision
        radius, bool scaled=true, int n=100 ){
      if( pIndex > 2){
        MatrixXp X(0,0);
        return X;
      }
      return getProjectionBallShadow( getProjector(pIndex), radius, scaled, n );
    }; 

    void notifyObservers(const VectorXp &x, int pIndex){
      for(int i=0; i<observers.size(); i++){
        observers[i]->projectionChanged(this, x, pIndex);
      }
    };

    virtual void dataChanged(Data<TPrecision> *data, typename DataObserver<TPrecision>::EventType type){
      if(type == DataObserver<TPrecision>::DATA){
        updateProjected();
      }
    };

    void updateProjected(){
      X = MatrixXp::Zero(2, data.getNumberOfPoints() );

      for(int i=0; i<X.cols(); i++){
        X.col(i) = project( data.getData(i) );
      }

      P = MatrixXp::Zero( 2, this->getNumberOfProjectors() );
      for(int i=0; i<P.cols(); i++){
        P.col(i) = project( this->getProjector(i) );
      }
    };


  protected:

    virtual void update( unsigned int i) = 0;
 
 
 


};





#endif
