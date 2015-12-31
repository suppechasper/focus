#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <vector>
#include <set>
#include <string>


#include "Eigen/Dense"

class RGB{
  public:
    RGB(float red=0, float green=0, float blue=0):r(red), g(green), b(blue){};

    static RGB blend(RGB &c1, RGB &c2, float alpha){
      RGB out;
      out.r = c1.r*alpha + c2.r* (1-alpha);
      out.g = c1.g*alpha + c2.g* (1-alpha);
      out.b = c1.b*alpha + c2.b* (1-alpha);
      return out;
    };

    float r;
    float g; 
    float b;
};

template<typename TPrecision> class Data;


template<typename TPrecision>
class DataObserver{
  public:
    enum EventType {DATA, GROUP, ACTIVE, DENSITY, SELECTION, COLORS, PAIRED, NAMES}; 
    virtual void dataChanged(Data<TPrecision> *data, EventType type) = 0;
};



template<typename TPrecision>
class Data{


  public:
 
    typedef typename DataObserver<TPrecision>::EventType EventType;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
 

  private:
 
    std::vector< DataObserver<TPrecision> *> observers;


  protected:
    
    //names of rows and columns
    std::vector<std::string> rownames;
    std::vector<std::string> colnames;
    
    RGB standardColor;
    
    int selected;

    std::vector<int> selection;


    //group id of each point
    std::vector<int> groupID;

    //status of each point
    std::vector< bool > active;

    //density
    std::vector< TPrecision > density;

    //Color of for each group
    std::vector< RGB> colors;

    std::vector< std::set<int> > paired;
  
    std::vector< std::vector<int> > lines;



  public:    
    
    Data(){
      using namespace Eigen;


      selected = -1;
      //setup colors
      standardColor = RGB(1, 1, 1);

      float r[9] = {0.8941176, 0.2156863, 0.3019608, 0.5960784, 1.0, 1.0, 0.6509804, 0.9686275, 0.6};
      static float g[9] = {0.1019608, 0.4941176, 0.6862745, 0.3058824, 0.4980392, 1.0, 0.3372549, 0.5058824, 0.6};
      static float b[9] = {0.1098039, 0.7215686, 0.2901961, 0.6392157, 0.0, 0.2, 0.1568627, 0.7490196, 0.6};

      for(int i=0; i<9; i++){
        colors.push_back(RGB(r[i], g[i], b[i]));
      }

    };


    virtual ~Data(){
    };


    virtual void addLine(std::vector<int> &l){
      lines.push_back(l);
    }

    virtual std::vector< std::vector<int> > &getLines(){
      return lines;
    };

    virtual void addObserver(DataObserver<TPrecision> *obs){ 
      observers.push_back(obs);
    };

    virtual void notifyObservers(EventType type){
      for(unsigned int i=0; i<observers.size(); i++){
        observers[i]->dataChanged(this, type);
      }
    }


    virtual int getDimension() = 0;

    virtual int getNumberOfPoints() = 0;
    

    virtual VectorXp getData(int i) = 0;

    virtual TPrecision getMin() = 0; 
    virtual TPrecision getMax() = 0; 

    virtual VectorXp getUpperBound(int i){
      return getData(i);
    };
    
    virtual VectorXp getLowerBound(int i){
      return getData(i);
    };

    virtual void resizeMetaData(int n){
      groupID.resize(n, -1);
      paired.resize( n );
      active.resize(n, true);

    };


    virtual std::string rowName(int i){
      if(i >= 0 && i <rownames.size() ){
        return rownames[i];
      }
      return "";
    };

    virtual std::string colName(int i){
      if(i >= 0 && i <colnames.size() ){
        return colnames[i];
      }
      return "";
    };



    virtual void setRowNames(std::vector<std::string> &rn){
      rownames= rn;
      notifyObservers(DataObserver<TPrecision>::NAMES);
    }
    virtual void setColNames(std::vector<std::string> &cn){
      colnames= cn;
      notifyObservers(DataObserver<TPrecision>::NAMES);
    }

    virtual void setColName(int i, std::string &s){
      if(colnames.size() < i){
        colnames.resize(i, "");
      }
      colnames[i] = s;
      notifyObservers(DataObserver<TPrecision>::NAMES);
    };


    virtual void setRowName(int i, std::string &s){
      if(rownames.size() < i){
        rownames.resize(i, "");
      }
      rownames[i] = s;
      notifyObservers(DataObserver<TPrecision>::NAMES);
    };

    virtual RGB &getDefaultColor(){
      return standardColor;
    };

    virtual void setDefaultColor(RGB &col){
      standardColor = col;
    };
    
    virtual void setColor(unsigned int gid, RGB &col){
      if( gid >= colors.size() ){
        colors.resize(gid+1, standardColor);
      }
      colors[gid] = col;
      notifyObservers(DataObserver<TPrecision>::COLORS);
    };

    virtual RGB &getColor(int i){
      if(groupID[i] < 0  || groupID[i] >= colors.size() ){
        return standardColor;
      }
      return colors[groupID[i]];
    };

    virtual RGB &getGroupColor(int gid){
      if(gid < 0  || gid >= colors.size() ){
        return standardColor;
      }
      return colors[gid];

    };

    virtual int getNumberOfGroups(){
      return colors.size();
    };

    virtual std::set< int > &getPaired(int i){
      return paired[i];
    };



    virtual void clearPaired(){
      for(unsigned int i=0; i< paired.size(); i++){
        paired[i].clear();
      }
      notifyObservers(DataObserver<TPrecision>::PAIRED);

    };




    virtual void addPaired(std::vector<int> &from, std::vector<int> &to, bool bothDirections=true){
      for(unsigned int i=0; i< from.size(); i++){
        if(from[i] < 0 || from[i] >= getNumberOfPoints() ){
          continue; 
        }
        if(to[i] < 0 || to[i] >= getNumberOfPoints() ){
          continue; 
        }
        paired[from[i]].insert( to[i] );
        if(bothDirections){
          paired[to[i]].insert( from[i] );
        }
      }
      notifyObservers(DataObserver<TPrecision>::PAIRED);
    };



    virtual void clearSelection(){
      selection.clear();
      notifyObservers(DataObserver<TPrecision>::SELECTION);
    };

    virtual void addSelected(int index){
      selection.push_back(index);
      notifyObservers(DataObserver<TPrecision>::SELECTION);
    };

    virtual void addSelected(std::vector<int> &indices){
      for(unsigned int i=0; i<indices.size(); i++){
        addSelected(indices[i]);
      }
      notifyObservers(DataObserver<TPrecision>::SELECTION);
    };
   
    virtual std::vector<int> &getSelection(){
      return selection;
    };
  
    virtual void setSelected(int s){
      selected =s;
      notifyObservers(DataObserver<TPrecision>::SELECTION);
    };

    virtual int getSelected(){
     return selected;
    };




    virtual VectorXp getSelectionMean(){
      std::vector<int> &sel = getSelection();
      VectorXp mean = VectorXp::Zero( getDimension());

      TPrecision w = 0;
      for(unsigned int i=0; i<sel.size(); i++){
        //if( isActive(sel[i]) ){
          mean += getData(sel[i]) * getDensity(sel[i]);
          w += getDensity(sel[i]);
        //}P:
      }
      if(w>0){
        mean /= w;
      }
      return mean;
    };




    virtual VectorXp getSelectionUpperBound(){
      std::vector<int> &sel = getSelection();
      if(sel.size() != 0){
        VectorXp upper = getSelectionMean();

        for(unsigned int i=0; i<sel.size(); i++){
          if( isActive(sel[i]) ){
            VectorXp tmp = getUpperBound(sel[i]);
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





    virtual VectorXp getSelectionLowerBound(){
      std::vector<int> &sel = getSelection();
      if(sel.size() != 0){
        VectorXp lower = getSelectionMean();

        for(unsigned int i=0; i<sel.size(); i++){
          if( isActive(sel[i]) ){
            VectorXp tmp = getLowerBound(sel[i]);
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





   virtual int getGroup(int index){
    return groupID[index];
   }; 
   

   virtual std::vector<int> getGroups(){
    return groupID;
   };




   virtual void setGroup(int gid, std::vector<int> &group, bool notify=true){
      if(gid <0 ){
        return;
      };

      for(unsigned int i=0; i<group.size(); i++){
        if(group[i] < 0 || group[i] >= getNumberOfPoints() ){
          continue;
        };
        groupID[group[i]] = gid ;
      }
      if(notify){
        notifyObservers(DataObserver<TPrecision>::GROUP);
      }
    };
   

    virtual void clearGroup(int gid, bool notify=true){
      for(int i=0; i<groupID.size(); i++){
        if(groupID[i] == gid){
          groupID[i] = -1;
        }
      }
      if(notify){
        notifyObservers(DataObserver<TPrecision>::GROUP);
      }
    };


    virtual std::vector<int> getIndex(std::string name){
      std::vector<int> ind;
      for(int i=0; i<getNumberOfPoints(); i++){
        if( name.compare(colName(i)) == 0 ){
          ind.push_back(i);
        }
      }
      return ind;
    };


    virtual void setActive(std::vector<int> &indices){
      for(unsigned int i=0; i<active.size(); i++){
        active[i] = false;
      }
      for(unsigned int i=0; i<indices.size(); i++){
        active[indices[i]] = true;
      }
      notifyObservers(DataObserver<TPrecision>::ACTIVE);
    };


    virtual void clearActive(){
      std::vector<int> all(active.size());
      for(unsigned int i=0; i<active.size(); i++){
        active[i] = true;
        all[i] = i;
      }
      notifyObservers(DataObserver<TPrecision>::ACTIVE);
    };



    virtual bool isActive(int index){
      return active[index];
    };



    virtual void setDensity(std::vector<TPrecision> &d){
      density = d;
      notifyObservers(DataObserver<TPrecision>::DENSITY);

    };


    virtual TPrecision getDensity(int index){
      if(density.size() == 0){
        return 1.0;
      }
      else{
        return density[index];
      }
    };



    virtual const std::vector<int> &getOriginalIndices(int i){
      static std::vector<int> ind(1);
      ind[0] = i;
      return ind;
    };




};




#endif
