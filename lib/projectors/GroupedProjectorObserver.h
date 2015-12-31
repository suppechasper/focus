#ifndef GROUPEDPROJECTOROBSERVER_H
#define GROUPEDPROJECTOROBSERVER_H

#include <set>

#include "ProjectorObserver.h"

template<typename TPrecision>
class GroupedProjectorObserver : public ProjectorObserver<TPrecision> {


  private:
    std::set< Projector<TPrecision> *> grouped;
    bool isUpdating;  

  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
    
    GroupedProjectorObserver(){
      isUpdating = false;
    };

    void addProjector(Projector<TPrecision> *p){
      int pIndex = 0;

      typename std::set< Projector<TPrecision> *>::iterator it = grouped.begin();
      for(; it !=grouped.end(); ++it){
         Projector<TPrecision> *projector = *it;
         for(;pIndex < projector->getNumberOfProjectors(); pIndex++){
           
           if(p->getDimension() != projector->getDimension() ){
             return;
           }

           p->setProjector( projector->getProjector(pIndex), pIndex );
         }
      }

      grouped.insert(p);
    };

    virtual void projectionChanged(Projector<TPrecision> *proj, const VectorXp &x, int pIndex) {

      if(!isUpdating){
        isUpdating=true;
        typename std::set< Projector<TPrecision> *>::iterator it = grouped.begin();
        for(; it !=grouped.end(); ++it){
          if( (*it) != proj){
            (*it)->setProjector(x, pIndex);
          }
        }
        isUpdating=false;
      }

    };

};





#endif
