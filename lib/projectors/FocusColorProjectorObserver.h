#ifndef FOCUSCOLORPROJECTOROBSERVER_H
#define FOCUSCOLORPROJECTOROBSERVER_H

#include "ProjectorObserver.h"
#include "DataInterface.h"

template<typename TPrecision>
class FocusColorProjectorObserver : public ProjectorObserver<TPrecision>{

  private:
    int index;
  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
   
    FocusColorProjectorObserver(int ind=2) : index(ind){

    };

    virtual ~FocusColorProjectorObserver(){};

    virtual void projectionChanged(Projector<TPrecision> *proj, const VectorXp &x, int pIndex){
      if(pIndex == index){
        Data<double> *data = &proj->getData();
        
        DataProxy<double> *proxy = dynamic_cast<DataProxy<double> *>(data);
        if(proxy != NULL){
          //std::cout << "using proxy" << std::endl;
          data = &proxy->getProxy();
        }
        VectorXp xs = data->getSelectionMean();
        VectorXp d = VectorXp::Zero( data->getNumberOfPoints() );
        for(int i=0; i<d.size(); i++){
          d(i) = ( data->getData(i) - xs ).norm();
        }
        TPrecision maxD = 1.001 * d.maxCoeff();

        int ng = data->getNumberOfGroups();
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
