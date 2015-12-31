#ifndef STANDARDDATAPROXY_H
#define STANDARDDATAPROXY_H

#include "DataProxy.h"

#include "Eigen/Dense"



template<typename TPrecision>
class StandardDataProxy : public DataProxy<TPrecision>{


  public:
 
    typedef typename DataObserver<TPrecision>::EventType EventType;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
 



  private:
    MatrixXp X;
    TPrecision rMin, rMax;
    
    std::vector< DataObserver<TPrecision> *> observers;
  
  public:    
    
    StandardDataProxy(MatrixXp &proxy, StandardData<TPrecision> &d) 
                                     : DataProxy<TPrecision>(d), X(proxy){
      rMin = X.minCoeff();
      rMax = X.maxCoeff();
    };


    virtual ~StandardDataProxy(){
    };


    virtual int getDimension(){
      return X.rows();
    }

    virtual VectorXp getData(int i){
      return X.col(i);
    };
   
    virtual TPrecision getMin(){
      return rMin;
    };
    
    virtual TPrecision getMax(){
      return rMax;
    };


    virtual VectorXp getUpperBound(int i){
      return getData(i);
    };
    
    virtual VectorXp getLowerBound(int i){
      return getData(i);
    };

  
    VectorXp getSelectionMean(){
      std::vector<int> &sel = this->getSelection();
      VectorXp mean = VectorXp::Zero( getDimension());

      TPrecision w = 0;
      for(unsigned int i=0; i<sel.size(); i++){
        //if( isActive(sel[i]) ){
          mean += getData(sel[i]) * this->getDensity(sel[i]);
          w += this->getDensity(sel[i]);
        //}P:
      }
      if(w>0){
        mean /= w;
      }
      return mean;
    };




    VectorXp getSelectionUpperBound(){
      std::vector<int> &sel = this->getSelection();
      if(sel.size() != 0){
        VectorXp upper = this->getSelectionMean();

        for(unsigned int i=0; i<sel.size(); i++){
          if( this->isActive(sel[i]) ){
            VectorXp tmp = this->getUpperBound(sel[i]);
            for(unsigned int j=0; j<upper.size(); j++){
              upper(j) = std::max( upper(j), tmp(j) );
            }
          }
        }
        return upper;
      }
      static VectorXp dummy = VectorXp::Zero( getDimension());
      return dummy;
    };





    VectorXp getSelectionLowerBound(){
      std::vector<int> &sel = this->getSelection();
      if(sel.size() != 0){
        VectorXp lower = this->getSelectionMean();

        for(unsigned int i=0; i<sel.size(); i++){
          if( this->isActive(sel[i]) ){
            VectorXp tmp = this->getLowerBound(sel[i]);
            for(unsigned int j=0; j < lower.size(); j++){
              lower(j) = std::min( lower(j), tmp(j) );
            }
          }
        }
        return lower;
      }
      static VectorXp dummy = VectorXp::Zero( getDimension());
      return dummy;
    };





};




#endif
