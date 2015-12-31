#ifndef GMRADATA_H
#define GMRADATA_H

#include <vector>


#include "StandardData.h"


template<typename TPrecision>
class GMRAData : public StandardData<TPrecision>{


  public:    




    GMRAData(){
    };


    virtual ~GMRAData(){};




    virtual void deleteNode(int index) = 0;


    virtual void expandNodes(std::vector<int> indices) = 0;


    virtual void expandAll(std::vector<int> &indices) = 0;


    virtual void expandToScale(int scale) = 0;


    virtual void collapseNodes(std::vector<int> &indices) = 0;


    virtual std::vector< int > getNodePoints( int index ) = 0;


};




#endif
