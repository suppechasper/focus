#ifndef FOCUSCOLORDATAOBSERVER_H
#define FOCUSCOLORDATAOBSERVER_H

#include "DataInterface.h"

template<typename TPrecision>
class FocusColorDataObserver : public DataObserver<TPrecision>{

  private:
    int index;
    int selected = -1;
  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
   
    FocusColorDataObserver(int ind=2) : index(ind){
    };

    virtual ~FocusColorDataObserver(){};

    virtual void dataChanged(Data<TPrecision> *data, EventType type ){
      if(type = DataObserver<double>::SELECTION){

        VectorXp = data->getSelectionMean();
        VectorXp d = VectorXp::Zero( data->getNumberOfPoints() );
        for(int i=0; i<d.size(); i++){
          d(i) = ( data->getData(i) - x ).norm();
        }
        TPrecision maxD = 1.001 * d.maxCoeff();

        int ng = data.getNumberOfGroups();
        std::vector< std::vector<int> > groups(ng);

        for(int i=0; i<d.size(); i++){
          for(int k=1; k<=ng; k++){
            if( d(i) < ( (k*maxD)/ng ) ){
              groups[k-1].push_back(i);
              break;
            }
          }
        }

        for(int k=0; k<groups.size(); k++){
          data->setGroup(k, groups[k], false);
        }
        data->notifyObservers(DataObserver<TPrecision>::GROUP);
      }
      
    };

};





#endif
