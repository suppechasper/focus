#ifndef PROJECTOROBSERVER_H
#define PROJECTOROBSERVER_H

template<typename TPrecision> class Projector;

template<typename TPrecision>
class ProjectorObserver{

  public:
    
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;
   
    virtual ~ProjectorObserver(){};

    virtual void projectionChanged(Projector<TPrecision> *proj, const VectorXp &x, int pIndex) = 0;

};





#endif
