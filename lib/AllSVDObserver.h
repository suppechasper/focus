#ifndef ALLSVDOBSERVER_H
#define ALLSVDOBSERVER_H

#include "SVDObserver.h"
#include "EigenRandomSVD.h"

template<typename TPrecision>
class AllSVDObserver : public SVDObserver<TPrecision>{
  private:

    EigenLinalg::RandomSVD<TPrecision> svd;
    bool center;
    int maxDim;
    int nPower;




  public:
    typedef typename DataObserver<TPrecision>::EventType EventType;
  
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    


    AllSVDObserver(bool c = true, int maxD=25, int nP = 0) : center(c), maxDim(maxD), nPower(nP){
    };



    void dataChanged(Data<TPrecision> *data, EventType type){  

      if(type == DataObserver<TPrecision>::DATA){
        VectorXp mean = VectorXp::Zero(data->getDimension());
        MatrixXp X(data->getDimension(), data->getNumberOfPoints());
        for(int i=0; i<data->getNumberOfPoints(); i++){
          X.col(i) =  data->getData(i);
          if(center){
            mean += X.col(i);
          }
        }
        if(center){
          mean /= data->getNumberOfPoints();
          X.colwise() -= mean;
        }
        this->svd = EigenLinalg::RandomSVD<TPrecision>(X, maxDim, nPower);
      }

    };
    
    Eigen::Ref<MatrixXp> getU(){
      return svd.GetU();
    };
    
    Eigen::Ref<VectorXp> getS(){
      return svd.GetS();
    };


};





#endif
