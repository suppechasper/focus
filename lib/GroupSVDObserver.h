#ifndef GROUPSVDOBSERVER_H
#define GROUPSVDOBSERVER_H

#include "SVDObserver.h"
#include "EigenRandomSVD.h"

template<typename TPrecision>
class GroupSVDObserver : public SVDObserver<TPrecision>{
  
  private:
    
    EigenLinalg::RandomSVD<TPrecision> svd;
    int groupID;
    bool center;
    int maxDim;
    int nPower;
 
 
  public:
    typedef typename DataObserver<TPrecision>::EventType EventType;
  
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    


    GroupSVDObserver(int gID, bool c = true, int maxD=25, int nP = 0) : 
      groupID(gID), center(c), maxDim(maxD), nPower(nP){
    };



    void dataChanged(Data<TPrecision> *data, EventType type){  

      if(type == DataObserver<TPrecision>::DATA || type == DataObserver<TPrecision>::GROUP){
        VectorXp mean = VectorXp::Zero( data->getDimension() );
        MatrixXp X(data->getDimension(), data->getNumberOfPoints());
        int index = 0;
        for(int i=0; i<data->getNumberOfPoints(); i++){
          if(data->getGroup(i) == groupID){
            X.col(index) =  data->getData(i);
            if(center){
              mean += X.col(index);
            }
            index++;
          }
        }
        if(center){
          mean /= index;
          X.colwise() -= mean;
        }
        if(index > 0){
          svd = EigenLinalg::RandomSVD<TPrecision>(X.leftCols(index), maxDim, nPower);
        }
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
