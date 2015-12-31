#ifndef SPARSEDATA_H
#define SPARSEDATA_H

#include "DataInterface.h"

#include "Eigen/Sparse"

template<typename TPrecision>
class SparseData : public Data<TPrecision>{

  public:
    typedef DataObserver<TPrecision> DO;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
 
    typedef typename Eigen::SparseMatrix<TPrecision> SparseMatrixXp;
    typedef typename Eigen::SparseVector<TPrecision> SparseVectorXp;

  protected:

    //raw data
    SparseMatrixXp raw;
    TPrecision rMin, rMax;


    

  public:    

    SparseData(){
    };

    SparseData(const SparseMatrixXp &X) {
      setData(X);
    };

    virtual ~SparseData(){
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


    void setData(const SparseMatrixXp &r, bool resizeMetaData= true){
      using namespace Eigen;

      raw = r;
      TPrecision *v = raw.valuePtr();

      rMin = v[0];
      rMax = v[0];
      for(int i=1; i<raw.nonZeros(); i++){
        if(rMin > v[i]){
          rMin = v[i];
        }
        else if(rMax < v[i]){
          rMax = v[i];
        }
      }

      if(resizeMetaData){
        this->resizeMetaData(raw.cols());
      }
      this->notifyObservers(DO::DATA);
    
    };

    
    
};




#endif
