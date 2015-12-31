#ifndef DATAPROXY_H
#define DATAPROXY_H

#include "DataInterface.h"

#include "Eigen/Dense"


template<typename TPrecision>
class NotifyDataObserver : public DataObserver<TPrecision>{
  
  private:
    Data<TPrecision> *data;
    std::vector< DataObserver<TPrecision> *> observers;

  public:
    typedef typename DataObserver<TPrecision>::EventType EventType;

    NotifyDataObserver(Data<TPrecision> *d):data(d){};

    virtual void dataChanged(Data<TPrecision> *d, EventType type){
      notifyObserver(type);
    };
    
    void notifyObserver(EventType type){
      for(unsigned int i=0; i<observers.size(); i++){
        observers[i]->dataChanged(data, type);
      }
    };
    
    void addObserver(DataObserver<TPrecision> *obs){ 
      observers.push_back(obs);
    };
};





template<typename TPrecision>
class DataProxy : public Data<TPrecision>{


  public:
 
    typedef typename DataObserver<TPrecision>::EventType EventType;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
 


  private:
    NotifyDataObserver<TPrecision> internal;

  protected:
    Data<TPrecision> &data;
    
  
  public:    
    
    DataProxy(Data<TPrecision> &d) : internal(this), data(d) {
      data.addObserver(&internal); 
    };


    virtual ~DataProxy(){
    };

    Data<TPrecision> &getProxy(){
      return data;
    };

    virtual void addLine(std::vector<int> &l){
      data.addLine(l);
    };

    virtual std::vector< std::vector<int> > &getLines(){
      return data.getLines();
    };

    virtual void addObserver(DataObserver<TPrecision> *obs){ 
      internal.addObserver(obs);
    };

    virtual void notifyObservers(EventType type){
      data.notifyObservers(type);
    };

    virtual int getDimension(){
      return data.getDimension();
    };

    virtual int getNumberOfPoints(){
      return data.getNumberOfPoints();
    };
    
    virtual VectorXp getData(int i){
      return data.getData(i);
    };
   
    virtual TPrecision getMin(){
      return data.getMin();
    };
    
    virtual TPrecision getMax(){
      return data.getMax();
    };


    virtual VectorXp getUpperBound(int i){
      return data.getUpperBound(i);
    };
    
    virtual VectorXp getLowerBound(int i){
      return data.getLowerBound(i);
    };

    void resizeMetaData(int n){
      data.resizeMetaData(n);
    };


    virtual std::string rowName(int i){
      return data.rowName(i);
    };

    virtual std::string colName(int i){
      return data.colName(i);
    };



    virtual void setRowNames(std::vector<std::string> &rn){
      data.setRowNames(rn);
    };

    virtual void setColNames(std::vector<std::string> &cn){
      data.setColNames(cn);
    };

    virtual void setColName(int i, std::string &s){
      data.setColName(i, s);
    };


    virtual void setRowName(int i, std::string &s){
      data.setRowName(i, s);
    };

    virtual RGB &getDefaultColor(){
      return data.getDefaultColor();
    };

    virtual void setDefaultColor(RGB &col){
      data.setDefaultColor(col);
    };
    
    virtual void setColor(unsigned int gid, RGB &col){
      data.setColor(gid, col);
    };

    virtual RGB &getColor(int i){
      return data.getColor(i);
    };

    virtual RGB &getGroupColor(int gid){
      return data.getGroupColor(gid);
    };


    virtual std::set< int > &getPaired(int i){
      return data.getPaired(i);
    };


    virtual void clearPaired(){
      data.clearPaired();
    };


    virtual void addPaired(std::vector<int> &from, std::vector<int> &to, bool bothDirections=true){
      data.addPaired(from, to, bothDirections);
    };


    virtual void clearSelection(){
      data.clearSelection();
    };

    virtual void addSelected(int index){
      data.addSelected(index);
    };

    virtual void addSelected(std::vector<int> &indices){
      data.addSelected(indices);
    };
   
    virtual std::vector<int> &getSelection(){
      return data.getSelection();
    };
  
    virtual void setSelected(int s){
      data.setSelected(s);
    };

    virtual int getSelected(){
     return data.getSelected();
    };




    virtual VectorXp getSelectionMean(){
      return data.getSelectionMean(); 
    };




    virtual VectorXp getSelectionUpperBound(){
      return data.getSelectionUpperBound();
    };




    virtual VectorXp getSelectionLowerBound(){
      return data.getSelectionLowerBound();
    };





   virtual int getGroup(int index){
    return data.getGroup(index);
   }; 
   

   virtual std::vector<int> getGroups(){
    return data.getGroups();
   };



   virtual void setGroup(int gid, std::vector<int> &group){
     data.setGroup(gid, group);      
    };
   

    virtual void clearGroup(int gid){
      data.clearGroup(gid);
    };


    virtual std::vector<int> getIndex(std::string name){
      return data.getIndex(name);
    };


    virtual void setActive(std::vector<int> &indices){
      data.setActive(indices);
    };


    virtual void clearActive(){
      data.clearActive();
    };



    virtual bool isActive(int index){
      return data.isActive(index);
    };



    virtual void setDensity(std::vector<TPrecision> &d){
      data.setDensity(d);

    };


    virtual TPrecision getDensity(int index){
      return data.getDensity(index);
    };



    virtual const std::vector<int> &getOriginalIndices(int i){
      return data.getOriginalIndices(i);
    };




};




#endif
