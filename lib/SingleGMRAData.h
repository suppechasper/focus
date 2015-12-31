#ifndef SINGLEGMRADATA_H
#define SINGLEGMRADATA_H

#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "GMRATree.h"
#include "GMRAData.h"
#include "EigenRandomSVD.h"

template<typename TPrecision>
class UpperBoundsVisitor : public Visitor<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    VectorXp bounds;

  public:
    UpperBoundsVisitor(VectorXp &b):bounds(b){};

    //
    virtual void visit(GMRANode<TPrecision> *node){
      if(node->getChildren().size() == 0){
        VectorXp &x = node->getCenter();
        for(int i=0;i<x.size(); i++){
          if(bounds(i) < x(i)){
            bounds(i) = x(i);
          }
        }
      }
    };

    VectorXp &getBound(){
      return bounds;
    };

};

template<typename TPrecision>
class LowerBoundsVisitor : public Visitor<TPrecision>{

  private:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

    VectorXp bounds;

  public:
    LowerBoundsVisitor(VectorXp &b):bounds(b){};

    //
    virtual void visit(GMRANode<TPrecision> *node){
      if(node->getChildren().size() == 0){
        VectorXp &x = node->getCenter();
        for(int i=0;i<x.size(); i++){
          if(bounds(i) > x(i)){
            bounds(i) = x(i);
          }
        }
      }
    };

    VectorXp &getBound(){
      return bounds;
    };

};





template<typename TPrecision>
class SingleGMRAData : public GMRAData<TPrecision>{

  public:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
  
  private:


    //raw data
    GMRATree<TPrecision> *gmra;
    std::vector< GMRANode<TPrecision> *> nodes;
    std::map< GMRANode<TPrecision> *, int>  node2index;
    int nCor;


  public:    

    SingleGMRAData( GMRATree<TPrecision> *tree, RGB coarse, RGB fine, int nC) :
           gmra(tree), nCor(nC) { 
      
      class MaxScaleVisitor : public Visitor<TPrecision>{
        public:
          int scale;
          MaxScaleVisitor(){
            scale= 0;
          };

          void visit(GMRANode<TPrecision> *node){
            scale = std::max(scale, node->getScale() );
          };
      };

      MaxScaleVisitor maxScale;
      gmra->breadthFirstVisitor(&maxScale);


      RGB red(1, 0, 0);
      
      this->colors.clear();
      for(int i=0; i<= maxScale.scale ; i++){
        this->colors.push_back( RGB::blend( fine, coarse, i/ ((float)maxScale.scale) ) );
      }


      nodes = gmra->getRoot()->getChildren();
      nodes.push_back( gmra->getRoot() );
      this->resizeMetaData( nodes.size() );
      
      this->active[ this->active.size() - 1 ] = false;

      updateData();
    };



    virtual ~SingleGMRAData(){
    };



    virtual VectorXp getUpperBound(int i){
      
      GMRANode<TPrecision> *node = nodes[i];
      UpperBoundsVisitor<TPrecision> bound(node->getCenter() );
      GMRATree<TPrecision>::depthFirst(&bound, node);
      return bound.getBound();
      
    };
    


    virtual VectorXp getLowerBound(int i){
      GMRANode<TPrecision> *node = nodes[i];
      LowerBoundsVisitor<TPrecision> bound(node->getCenter() );
      GMRATree<TPrecision>::depthFirst(&bound, node);
      return bound.getBound();
    };



    GMRATree<TPrecision> *getGMRA(){
       return gmra;
    };


/*
    virtual VectorXp getScoreUpperBound(int index){
      CollectLeavesVisitor<TPrecision> cl;
      GMRATree<TPrecision>::depthFirst(&cl, nodes[index]);
      
      std::vector< GMRANode<TPrecision> *> &leaves = cl.getLeaves();
      
      VectorXp upper = nodes[index]->getCenter().head(nCor) ;


      for(unsigned int i=0; i<leaves.size(); i++){
        VectorXp &tmp = leaves[i]->getCenter() ;
        for(unsigned int j=0; j<upper.size(); j++){
          upper(j) = std::max( upper(j), tmp(j) );
        }
      }
      return upper;
    };


    
    virtual VectorXp getScoreLowerBound(int index){
      CollectLeavesVisitor<TPrecision> cl;
      GMRATree<TPrecision>::depthFirst(&cl, nodes[index]);
      
      std::vector< GMRANode<TPrecision> *> &leaves = cl.getLeaves();
      
      VectorXp lower = nodes[index]->getCenter().head(nCor) ;


      for(unsigned int i=0; i<leaves.size(); i++){
        VectorXp &tmp = leaves[i]->getCenter() ;
        for(unsigned int j=0; j<lower.size(); j++){
          lower(j) = std::min( lower(j), tmp(j) );
        }
      }
      return lower;
    };


*/




   void deleteNode(int index){
     if( !this->active[index] ){
       return;
     }

     GMRANode<TPrecision> *node = nodes[index];
     nodes.clear();
     node2index.erase(node);

     std::vector< bool > activeUpdate;
     for(typename std::map< GMRANode<TPrecision> *, int>::iterator it =
         node2index.begin(); it != node2index.end(); ++it){
       nodes.push_back(it->first);
       activeUpdate.push_back( this->active[it->second] );
     }
     this->active = activeUpdate; 
     

     GMRANode<TPrecision> *parent = node->getParent();
     if(parent != NULL){
       parent->removeChild(node);
     }

     DeleteVisitor<TPrecision> del;
     GMRATree<TPrecision>::depthFirst(&del, node);

     while(parent != NULL){
       std::vector<int> &pts = parent->getPoints();
       pts.clear();          
       //hacky
       VectorXp &mean = parent->getCenter();
       mean *= 0;

       int nPts = 0;
       std::vector<GMRANode<TPrecision> *> &chitlums = parent->getChildren();
       for(int i=0; i<chitlums.size(); i++){
         std::vector<int> &tmp = chitlums[i]->getPoints();
         pts.insert( pts.end(), tmp.begin(), tmp.end() );
         
         nPts += tmp.size();
         mean += chitlums[i]->getCenter() * tmp.size();
       }
       
       mean /= nPts;
       mean.head(nCor).normalize();
       //parent->setCenter(mean);

       parent = parent->getParent();

     };
      
     this->selection.clear();
     this->selection.push_back( this->selected );

     updateData();
   
   };




    void expandNodes(std::vector<int> indices){

      for(unsigned int i=0; i<indices.size(); i++){
        int index = indices[i];
        GMRANode<TPrecision> *node = nodes[index];
        std::vector<GMRANode<TPrecision> * > kids = node->getChildren();

        for(unsigned int i=0; i<kids.size(); i++){

          typename std::map<GMRANode<TPrecision> *, int>::iterator it = node2index.find(kids[i]);
          if(it == node2index.end() ){

            nodes.push_back(kids[i]);

            this->active.push_back( true );
            //this->groupID.push_back( this->groupID[index] );
          }
        }

        if(kids.size() > 0 ){
          this->active[index] = false;
        }
      }

      updateData();

    };


    void expandAll(std::vector<int> &indices){
      for(unsigned int i=0; i<indices.size(); i++){
        int index = indices[i];
        GMRANode<TPrecision> *node = nodes[index];
      
        expandRecursive( node );      
      
        std::vector<GMRANode<TPrecision> * > kids = node->getChildren();
        if(kids.size() > 0 ){
          this->active[index] = false;
        }
      }


      updateData();

    };


    void expandToScale(int scale){
      nodes.clear();      
      this->active.clear();


      class ExpandToScaleVisitor : public Visitor<TPrecision>{
      
        public:
      
          int scale;
          std::vector<bool> &active;
          std::vector<GMRANode<TPrecision> *> &nodes;
          
          ExpandToScaleVisitor(int toScale, std::vector<bool> &a,
              std::vector<GMRANode<TPrecision> *> &n):active(a), nodes(n){ 
            scale = toScale;
          };

          void visit(GMRANode<TPrecision> *node){
            if( node->getScale() <= scale ){
              nodes.push_back(node);
              active.push_back( node->getScale() == scale || node->getChildren().size() == 0 );
            }
          };
      
      };

      ExpandToScaleVisitor expander(scale, this->active, nodes);
      gmra->breadthFirstVisitor( &expander );

      updateData();
    };







    void collapseNodes(std::vector<int> &indices){

      for(unsigned int i=0; i<indices.size(); i++){
        recurisveCollapseNode( nodes[ indices[i] ] );
      }
      nodes.clear();
      std::vector< bool > activeUpdate;
      //std::vector< int > groupUpdate;

      for( typename std::map<GMRANode<TPrecision> *, int>::iterator it = node2index.begin(); 
          it != node2index.end(); ++it){

        nodes.push_back( it->first );
        if( std::find( indices.begin(), indices.end(),  it->second ) != indices.end() ){
          this->selected = activeUpdate.size();
          activeUpdate.push_back( true );
        }
        else{
          activeUpdate.push_back( this->active[it->second] );
        }
       // groupUpdate.push_back( this->groupID[it->second] );
      }

      this->active = activeUpdate; 
      //this->groupID = groupUpdate;

     
      this->selection.clear();
      this->selection.push_back( this->selected );

      updateData();

    };




    std::vector< int > getNodePoints( int index ){
      static std::vector<int> empty;
      if(index < 0 || index >= nodes.size() ){
        return empty;

      }
      return nodes[index]->getPoints();
    };


    void updateData(){
      using namespace Eigen;


      this->raw = MatrixXp( nCor, nodes.size() );
      node2index.clear();
      this->groupID.resize(nodes.size());
      for(unsigned int i=0; i<nodes.size(); i++){
        this->raw.col(i) = nodes[i]->getCenter().head(nCor);
        node2index[ nodes[i] ] = i;
        this->groupID[i] = nodes[i]->getScale();
      }


      //update paired, i.e. siblings
      this->paired.clear();
      this->paired.resize(nodes.size() );
      for(unsigned int i=0; i<nodes.size(); i++){
        //GMRANode<TPrecision> *p = nodes[i]->getParent();
        //if(p != NULL){
          std::vector< GMRANode<TPrecision>* > kids = nodes[i]->getChildren();
          std::set< int > pairs;
          for(unsigned int j=0; j<kids.size(); j++ ){
            typename std::map<GMRANode<TPrecision> *, int>::iterator it = node2index.find(kids[j]);
            if(it != node2index.end() ){
              pairs.insert( it->second );
            }
          }
          
          GMRANode<TPrecision> *p = nodes[i]->getParent();
          if(p != NULL){
            typename std::map<GMRANode<TPrecision> *, int>::iterator it = node2index.find( p );
            if(it != node2index.end() ){
              pairs.insert( it->second );
            }
          }

          this->paired[i] = pairs;
        //}
      }


      this->setData(this->raw, false);

    };

    









  private:
    
    void recurisveCollapseNode(GMRANode<TPrecision> *node){
        
      std::vector< GMRANode<TPrecision> * > kids = node->getChildren();
        
      std::vector<bool> erased( kids.size(), false);
      for(int i=0; i<kids.size(); i++){      
        typename std::map<GMRANode<TPrecision> *, int>::iterator it = node2index.find(kids[i]);
        if(it!=node2index.end() ){
          node2index.erase( it );
          erased[i] =true;
        }
      }

      for(int i=0; i<erased.size(); i++){
        if(erased[i]){
          recurisveCollapseNode(kids[i]);
        }
      }

    };


    void expandRecursive(GMRANode<TPrecision> *node){
        
      std::vector<GMRANode<TPrecision> * > kids = node->getChildren();
        
      std::vector<bool> added( kids.size(), false);
      for(int i=0; i<kids.size(); i++){

        typename std::map<GMRANode<TPrecision> *, int>::iterator it = node2index.find(kids[i]);
        if(it == node2index.end() ){
          added[i] = kids[i]->getChildren().size() > 0;
          nodes.push_back(kids[i]);
          this->active.push_back( !added[i] );
        }
      }


      for(int i=0; i<added.size(); i++){
        if(added[i]){
          expandRecursive(kids[i]);
        }
      }

    };
};




#endif
