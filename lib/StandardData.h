#ifndef STANDARDDATA_H
#define STANDARDDATA_H


#include "DataInterface.h"


template<typename TPrecision>
class StandardData : public Data<TPrecision>{

  public:
    typedef DataObserver<TPrecision> DO;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
 

  protected:

    //raw data
    MatrixXp raw;
    TPrecision rMin, rMax;


    

  public:    

    StandardData(){
    };

    StandardData(const MatrixXp &X) {
      setData(X);
    };

    virtual ~StandardData(){
    };


    virtual int getDimension(){
      return raw.rows();
    };

    virtual int getNumberOfPoints(){
      return raw.cols();
    };
    

    VectorXp getData(int i){
      return raw.col(i);
    };

    virtual TPrecision getMin(){
      return rMin;
    };
    
    virtual TPrecision getMax(){
      return rMax;
    };


    void setData(const MatrixXp &r, bool resizeMetaData= true){
      using namespace Eigen;

      raw = r;
      rMin = raw(0,0);
      rMax= rMin;
      for(int i=0; i<getNumberOfPoints(); i++){
        VectorXp a = this->getUpperBound(i);
        VectorXp b = this->getLowerBound(i);
        
        rMax = std::max(rMax, a.maxCoeff());
        rMin = std::min(rMin, b.minCoeff());
      }

      if(resizeMetaData){
        this->resizeMetaData(raw.cols());
      }
      this->notifyObservers(DO::DATA);
    
    };

    
    
};




#endif
