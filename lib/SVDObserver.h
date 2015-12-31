#ifndef SVDOBSERVER_H
#define SVDOBSERVER_H

#include "Eigen/Dense"

template<typename TPrecision>
class SVDObserver : public DataObserver<TPrecision>{
  
    
 
 
  public:
  
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    


    virtual Eigen::Ref<MatrixXp> getU() = 0;
    virtual Eigen::Ref<VectorXp> getS() = 0;

};




#endif
