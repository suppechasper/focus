#ifndef NULL
#define NULL 0
#endif

#define R_NO_REMAP


#include <R.h>
#include <Rinternals.h>

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "Eigen/Dense"
#include "Eigen/Sparse"

#include "GMRATree.h"

#include "GlutStrokeFont.h"
#include "Focus.h"

#include "DataInterface.h"
#include "StandardData.h"
#include "StandardDataProxy.h"
#include "SparseData.h"
#include "SingleGMRAData.h"

//Data observers
#include "GroupSVDObserver.h"
#include "AllSVDObserver.h"

//Projectors
#include "OrthogonalProjector.h"
#include "HyperbolicOrthogonalProjector.h"

#include "CorrelationProjector.h"
#include "LinearProjector.h"

#include "SinglePointFocusProjector.h"
#include "HyperbolicSinglePointFocusProjector.h"

#include "TwoPointFocusProjector.h"
#include "HyperbolicTwoPointFocusProjector.h"

#include "LineFocusProjector.h"
#include "HyperbolicLineFocusProjector.h"

#include "ThreePointFocusProjector.h"

#include "HeatTriangulationProjector.h"

#include "GroupedProjectorObserver.h"

#include "RotationProjectorAnimator.h"
#include "TangentRotationProjectorAnimator.h"

#include "FocusColorProjectorObserver.h"

//Displays
#include "ProjectionDEL.h"
//background renderers for projection display
#include "CircleBackground.h"
#include "HorizontalLinesCircleBackground.h"
#include "VerticalLinesCircleBackground.h"
#include "CenterBallShadowBackground.h"
#include "ProjectorBallShadowBackground.h"

//other displays
#include "PCADEL.h"
#include "ProfileDEL.h"
#include "ImageDEL.h"
#include "MatrixDEL.h"
#include "SubMatrixDEL.h"
#include "OffsetMatrixDEL.h"
#include "PointsDEL.h"


  RGB getCoarse(int i){
    switch(i){
      case 0:
        return RGB(1, 1, 0);
      case 1:
        return RGB(0.5, 0.5, 1);
      case 2:
        return RGB(0.5, 1, 0.5);
      case 3:
        return RGB(1, 1, 0.5);
      case 4:
        return RGB(1, 0.5, 1);
      case 5:
        return RGB(0.5, 1, 1);
    }
    return RGB(1, 1, 1);
  }




  RGB getFine(int i){
    switch(i){
      case 0:
        return RGB(1, 0, 0);
      case 1:
        return RGB(0, 0, 1);
      case 2:
        return RGB(0, 1, 0);
      case 3:
        return RGB(1, 1, 0);
      case 4:
        return RGB(1, 0, 1);
      case 5:
        return RGB(0, 1, 1);
    }
    return RGB(1, 1, 1);
  }



extern "C" {


  pthread_t focusThread;

  int dataCounter;
  int projCounter;
  int delCounter;
  int bgCounter;

  bool stopFocus;
  bool updateFocus;
  bool isRunning;
  Focus<double> *focus;
  GlutStrokeFont font;

  pthread_mutex_t mutex;

  
  
  /* called from .onLoad */
  SEXP focus_load(void){
    pthread_mutex_init(&mutex, NULL);
   
    focus = new Focus<double>( font );

    dataCounter = 0;  
    projCounter = 0;  
    delCounter = 0;
    bgCounter = 0;
    stopFocus = false;
    updateFocus = false;
    isRunning = false;
    
    return R_NilValue;

  };


  SEXP focus_unload(void){
    
    stopFocus = true;
    
    return R_NilValue;
  };




  //---------------- Run focus in sperate thread

  //display methods
  int mainWindow;

  void display(void){
    pthread_mutex_lock(&mutex);
    focus->display();
    pthread_mutex_unlock(&mutex);
  };

  void idle(void){
    
    pthread_mutex_lock(&mutex);
    focus->idle();
    pthread_mutex_unlock(&mutex);
    
    if(stopFocus){
      stopFocus=false;
      glutLeaveMainLoop();
    }
    else if(updateFocus){
      updateFocus = false;
  
      pthread_mutex_lock(&mutex);
      focus->reshape();
      pthread_mutex_unlock(&mutex);

      glutPostRedisplay();
    }
  };



  void mouse(int button, int state, int x, int y){
    pthread_mutex_lock(&mutex);
    focus->mouse(button, state, x, y);
    pthread_mutex_unlock(&mutex);

  }

  void motion(int x, int y){
    pthread_mutex_lock(&mutex);
    focus->motion(x, y);
    pthread_mutex_unlock(&mutex);

  }

  void passive(int x, int y){
    pthread_mutex_lock(&mutex);
    focus->passive(x, y);
    pthread_mutex_unlock(&mutex);

  }

  void keyboard(unsigned char key, int x, int y){
    switch(key){
      case 27:   
      case 'q':
      case 'Q':
        glutLeaveMainLoop();
        break;
    }
    pthread_mutex_lock(&mutex);
    focus->keyboard(key, x, y);
    pthread_mutex_unlock(&mutex);

  }

  void special(int key, int x, int y){
    pthread_mutex_lock(&mutex);
    focus->special(key, x, y);
    pthread_mutex_unlock(&mutex);

  }

  void reshape(int w, int h){
    pthread_mutex_lock(&mutex);
    focus->reshape(w, h);
    pthread_mutex_unlock(&mutex);

    glutPostRedisplay();
  }



  void visible(int vis)
  {
    if (vis == GLUT_VISIBLE)
      glutIdleFunc(idle);
    else
      glutIdleFunc(NULL);
  };



  void *focusRun(void *arg){
    int argc = 1;
    char *title  = "focus";


    glutInit(&argc, &title );
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(1100, 710);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    mainWindow = glutCreateWindow(focus->title().c_str());


    glutDisplayFunc(display);
    glutVisibilityFunc(visible);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    focus->init();

    glutShowWindow();
    
    pthread_mutex_unlock(&mutex);
   
    try{ 
      glutMainLoop();
    }
    catch(char const *err){
      Rprintf(err);
    };

    delete focus;

    focus = new Focus<double>(font);
    isRunning=false;
    pthread_exit(NULL);
  };



  SEXP focusStart(){
    if(!isRunning){
      isRunning = true;
      pthread_mutex_lock(&mutex);
      pthread_create( &focusThread, NULL, focusRun, NULL);
    }
    return R_NilValue;
  };





  //-------------- Focus data setup methods
  
  
  Data<double> *getData(SEXP Rdata){     
    SEXP Rdata_pointer = Rf_getAttrib(Rdata, Rf_install("focus_data_ptr") );
    Data<double> *data = static_cast<Data<double> *>( R_ExternalPtrAddr(Rdata_pointer) );
    return data;
  };


  SEXP focusMatrixData( SEXP Rm, SEXP Rn, SEXP Rx) {
    using namespace Eigen;
    int m = *INTEGER(Rm);
    int n = *INTEGER(Rn);


    SEXP rl, cl;
    const char *rn, *cn;
    Rf_GetMatrixDimnames(Rx, &rl, &cl, &rn, &cn);
    double *x = REAL(Rx);

    std::vector<std::string> cnames;
    for(int i=0; i<n; i++){
      const char *name = CHAR(STRING_ELT(cl,i));
      cnames.push_back(name);
    }

    std::vector<std::string> rnames;
    for(int i=0; i<m; i++){
      const char *name = CHAR(STRING_ELT(rl,i));
      rnames.push_back(name);
    }

    Map<MatrixXd> Xtmp(x, m, n);
    MatrixXd X = Xtmp;



    StandardData<double> *data = new StandardData<double>(X);
    data->setColNames(cnames);
    data->setRowNames(rnames);


    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = dataCounter;
    SEXP ptr = R_MakeExternalPtr(data, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_data_ptr"), ptr); 

    ++dataCounter;


    UNPROTECT(2);
    return Rid;
  };



  SEXP focusMatrixDataProxy(SEXP Rdata,  SEXP Rm, SEXP Rn, SEXP Rx) {
    using namespace Eigen;
    int m = *INTEGER(Rm);
    int n = *INTEGER(Rn);
    double *x = REAL(Rx);

    Map<MatrixXd> Xtmp(x, m, n);
    MatrixXd X = Xtmp;


    Data<double> *tmp = getData(Rdata);

    StandardData<double> *sdata = dynamic_cast< StandardData<double> *>( tmp );
    if(sdata == NULL){
      return R_NilValue;
    };

    StandardDataProxy<double> *data = new StandardDataProxy<double>(X, *sdata);


    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = dataCounter;
    SEXP ptr = R_MakeExternalPtr(data, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_data_ptr"), ptr); 

    ++dataCounter;


    UNPROTECT(2);
    return Rid;
  };

  
  
  SEXP focusSparseMatrixData( SEXP Rm, SEXP Rn, SEXP Rnnz, SEXP Rrow, SEXP Rcol, SEXP Rx) {
    using namespace Eigen;
    int m = *INTEGER(Rm);
    int n = *INTEGER(Rn);
    int nnz = *INTEGER(Rnnz);

    double *x = REAL(Rx);
    int *rowIndex = INTEGER(Rrow);
    int *colIndex = INTEGER(Rcol);


    std::vector< Eigen::Triplet<double> > triplets(nnz);
    for(unsigned int i=0; i<triplets.size(); i++){
      triplets[i] = Eigen::Triplet<double>(rowIndex[i]-1, colIndex[i]-1, x[i]);
    }

    SparseMatrix<double> X(m, n);
    X.setFromTriplets(triplets.begin(), triplets.end());
    SparseData<double> *data = new SparseData<double>(X);


    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = dataCounter;
    SEXP ptr = R_MakeExternalPtr(data, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_data_ptr"), ptr); 

    ++dataCounter;


    UNPROTECT(2);
    return Rid;
  };




  //Setup focus gmra data object
  GMRATree<double> *getGMRA(SEXP Rgmra){     
    SEXP Rgmra_pointer = Rf_getAttrib(Rgmra, Rf_install("gmra_ptr") );
    GMRATree<double> *gmra = static_cast<GMRATree<double> *>( R_ExternalPtrAddr(Rgmra_pointer) );
    return gmra;
  };



  SEXP focusSingleGMRA(SEXP Rgmra, SEXP RnCor) { 

    using namespace Eigen;

    GMRATree<double> *gmra = getGMRA(Rgmra);

    int m = *INTEGER(RnCor);

    SingleGMRAData<double> *data = new SingleGMRAData<double>(gmra, getCoarse(0), getFine(0), m );
    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = dataCounter;
    SEXP ptr = R_MakeExternalPtr(data, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_data_ptr"), ptr); 

    ++dataCounter;

    UNPROTECT(2);
    return Rid;

  };










  //-------------- Focus projections setup

  
  Projector<double> *getProjector(SEXP Rp){     
    SEXP Rp_pointer = Rf_getAttrib(Rp, Rf_install("focus_proj_ptr") );
    Projector<double> *p = static_cast<Projector<double> *>( R_ExternalPtrAddr(Rp_pointer) );
    return p;
  };


  SEXP focusProjection(Projector<double> *p){  
   
    p->updateProjected();

    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = projCounter;
    SEXP ptr = R_MakeExternalPtr(p, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_proj_ptr"), ptr); 

    ++projCounter;

    UNPROTECT(2);
    return Rid;
  };



  SEXP focusOrthogonalProjection(SEXP Rdata, SEXP Rscaled, SEXP Rtripod, SEXP Rfixed, SEXP Rhyperbolic){
    Data<double> *data=getData(Rdata);

    Projector<double> *p;
    if( *INTEGER(Rhyperbolic) ){
      p = new HyperbolicOrthogonalProjector<double>( *data, *INTEGER(Rscaled),
          *INTEGER(Rtripod), *INTEGER(Rfixed) ); 
    }
    else{
      p = new OrthogonalProjector<double>( *data, *INTEGER(Rscaled),
        *INTEGER(Rtripod), *INTEGER(Rfixed) ); 
    }
    return focusProjection(p); 
  };


  SEXP focusLinearProjection(SEXP Rdata, SEXP Rscaled, SEXP Rfixed){
    Data<double> *data=getData(Rdata);
    
    LinearProjector<double> *p = new LinearProjector<double>( *data,
        *INTEGER(Rscaled),  *INTEGER(Rfixed) ); 
    return focusProjection(p); 
  };



  SEXP focusCorrelationProjection(SEXP Rdata){
    Data<double> *data=getData(Rdata);
    
    CorrelationProjector<double> *p = new CorrelationProjector<double>( *data );
    return focusProjection(p); 
  };





  SEXP focusSinglePointFocusProjection(SEXP Rdata, SEXP Rscaled, SEXP Rtripod, SEXP Rfixed, SEXP Rhyperbolic){
    Data<double> *data=getData(Rdata);
    
    Projector<double> *p;
    if( *INTEGER(Rhyperbolic) ){
     p = new HyperbolicSinglePointFocusProjector<double>( *data,
         *INTEGER(Rscaled), *INTEGER(Rtripod), *INTEGER(Rfixed) ); 
    }
    else{
      p = new SinglePointFocusProjector<double>( *data, *INTEGER(Rscaled),
          *INTEGER(Rtripod), *INTEGER(Rfixed) ); 
    }
    return focusProjection(p); 
  };


  SEXP focusTwoPointFocusProjection(SEXP Rdata, SEXP Rscaled, SEXP Rhyperbolic){
    Data<double> *data=getData(Rdata);
    
    Projector<double> *p;
    if( *INTEGER(Rhyperbolic) ){
      p = new HyperbolicTwoPointFocusProjector<double>( *data, *INTEGER(Rscaled) ); 
    }
    else{
      p = new TwoPointFocusProjector<double>( *data, *INTEGER(Rscaled) ); 
    }

    return focusProjection(p); 
  };


  SEXP focusLineFocusProjection(SEXP Rdata, SEXP Rhyperbolic){
    Data<double> *data=getData(Rdata);
    
    Projector<double> *p;
    if( *INTEGER(Rhyperbolic) ){ 
      p = new HyperbolicLineFocusProjector<double>( *data );
    }
    else{
      p = new LineFocusProjector<double>( *data );
    }
    return focusProjection(p); 
  };



  SEXP focusThreePointFocusProjection(SEXP Rdata){
    Data<double> *data=getData(Rdata);
    
    ThreePointFocusProjector<double> *p = new ThreePointFocusProjector<double>( *data );
    return focusProjection(p); 
  };


  SEXP focusHeatTriangulation(SEXP Rdata, SEXP Rhyperbolic){
    Data<double> *data=getData(Rdata);
    
    Projector<double> *p;
    if( *INTEGER(Rhyperbolic) ){ 
      p = new HeatTriangulationProjector<double>( *data );
    }
    else{
      p = new HeatTriangulationProjector<double>( *data );
    }
    return focusProjection(p); 
  };







  SEXP focusProjectionGroup(SEXP Rprojections){
    GroupedProjectorObserver<double> *group = new GroupedProjectorObserver<double>();

    for(int i=0; i<Rf_length(Rprojections); i++){
      SEXP Rproj = VECTOR_ELT(Rprojections, i);
      Projector<double> *p = getProjector(Rproj);
      if(p!=NULL){
        group->addProjector(p);
        p->addObserver(group);
      }
    }

    return R_NilValue; 
  };





  SEXP focusNoAnimation(SEXP Rproj){
    Projector<double> *p = getProjector(Rproj);

    ProjectorAnimator<double> *pAnim = new ProjectorAnimator<double>( );
    p->setProjectorAnimator(pAnim);
    
    return R_NilValue; 
  };


  SEXP focusRotationAnimation(SEXP Rproj){
    Projector<double> *p = getProjector(Rproj);

    RotationProjectorAnimator<double> *pAnim = new
      RotationProjectorAnimator<double>( focus->getAnimator() );
    p->setProjectorAnimator(pAnim);
    
    return R_NilValue; 
  };


  SEXP focusTangentRotationAnimation(SEXP Rproj, SEXP Rt1, SEXP Rt2){
    Projector<double> *p = getProjector(Rproj);

    int t1 = *INTEGER(Rt1) -1;
    int t2 = *INTEGER(Rt2) -1;

    TangentRotationProjectorAnimator<double> *pAnim = new
      TangentRotationProjectorAnimator<double>( focus->getAnimator(), t1, t2 );
    p->setProjectorAnimator(pAnim);
    
    return R_NilValue; 
  }


  SEXP focusAddFocusColor(SEXP Rproj, SEXP Rindex){
    Projector<double> *p = getProjector(Rproj);

    int index = *INTEGER(Rindex);
    FocusColorProjectorObserver<double> *fColor=new FocusColorProjectorObserver<double>(index);
    p->addObserver(fColor);
    
    return R_NilValue; 
  };



  //-------------- Focus display elements setup

  DisplayElement *getDel(SEXP Rdel){     
    SEXP Rdel_pointer = Rf_getAttrib(Rdel, Rf_install("focus_del_ptr") );
    DisplayElement *del = static_cast<DisplayElement *>( R_ExternalPtrAddr(Rdel_pointer) );
    return del;
  };



  SEXP addDisplayElement( DisplayElement *del, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, bool showBG){    
    
    pthread_mutex_lock(&mutex);
    
    del->location(*REAL(Rx), *REAL(Ry), *REAL(Rw), *REAL(Rh));
    del->setDrawBackground( showBG );
    focus->addDisplayElement(del);
    
    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = delCounter;
    SEXP ptr = R_MakeExternalPtr(del, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_del_ptr"), ptr); 


    UNPROTECT(2);  

    ++delCounter;

    updateFocus = true;
    pthread_mutex_unlock(&mutex);


    return Rid;

  };




  SEXP focusProjectionDEL(SEXP Rproj, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, SEXP RdrawPoints, SEXP RdrawLines, SEXP RpassiveSelect){
    if(focus == NULL){
      return R_NilValue;
    }
    
    Projector<double> *proj = getProjector(Rproj);

    bool drawPoints = *INTEGER(RdrawPoints);
    bool drawLines = *INTEGER(RdrawLines);
    bool passive = *INTEGER(RpassiveSelect);
    ProjectionDEL<double> *del = 
      new ProjectionDEL<double>(*proj, focus->getFont(), drawPoints, drawLines, passive );

    return addDisplayElement(del, Rx, Ry, Rw, Rh, true);

  };


  //Other display elements
  SEXP focusPCADEL(SEXP Rproj, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, SEXP RshowBG){
    if(focus == NULL){
      return R_NilValue;
    }
    
    Projector<double> *proj = getProjector(Rproj);
    Data<double> &data = proj->getData();

    AllSVDObserver<double> *allSVD = new AllSVDObserver<double>();
    data.addObserver(allSVD);
    allSVD->dataChanged(&data, DataObserver<double>::DATA);
    

    PCADEL<double> *del = new PCADEL<double>(allSVD, *proj, data.getDefaultColor() );
    return addDisplayElement(del, Rx, Ry, Rw, Rh, *INTEGER(RshowBG) );

  };  
    



  
  SEXP focusGroupPCADEL(SEXP Rproj, SEXP Rgid, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, SEXP RshowBG){
    if(focus == NULL){
      return R_NilValue;
    }
    
    Projector<double> *proj = getProjector(Rproj);
    Data<double> &data = proj->getData();
  
    GroupSVDObserver<double> *gSVD = new GroupSVDObserver<double>( *INTEGER(Rgid) );
    data.addObserver(gSVD);
    gSVD->dataChanged(&data, DataObserver<double>::DATA);

    PCADEL<double> *del = new PCADEL<double>(gSVD, *proj, data.getGroupColor( *INTEGER(Rgid) ) );
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  *INTEGER(RshowBG) );
  };  

  

  SEXP focusProfileDEL(SEXP Rdata, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, SEXP RshowBG){
    if(focus == NULL){
      return R_NilValue;
    }
    
    Data<double> *data = getData(Rdata);
  
    ProfileDEL<double> *del=new ProfileDEL<double>(*data, focus->getFont() );
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  *INTEGER(RshowBG) );
  };




  SEXP focusPointsDEL(SEXP Rdata, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, SEXP RshowBG){
    if(focus == NULL){
      return R_NilValue;
    }
    
    Data<double> *data = getData(Rdata);
  
    PointsDEL<double> *del=new PointsDEL<double>(*data, focus->getFont() );
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  *INTEGER(RshowBG) );
  };


  SEXP focusPointsDELaddPoints(SEXP Rdel, SEXP Rn, SEXP RX, SEXP Rid){
    DisplayElement *del = getDel(Rdel);
    PointsDEL<double> *pdel = dynamic_cast< PointsDEL<double>* >(del);
    if(pdel !=NULL){
      double *x = REAL(RX);
      int n = *INTEGER(Rn);
      int id = *INTEGER(Rid);
      Eigen::Map<Eigen::MatrixXd> X( x, 2, n );
      pdel->setPoints(id, X); 
    }

    return R_NilValue;

  };


  SEXP focusImageDEL(SEXP Rdata, SEXP Rimage, SEXP Rdims, SEXP Rind, SEXP Rx,
      SEXP Ry, SEXP Rw, SEXP Rh, SEXP RshowBG){  
    if(focus == NULL){
      return R_NilValue;
    }

    Data<double> *data = getData(Rdata);

    int *dims = INTEGER(Rdims);
    double *image = REAL(Rimage);  
    int *ind = INTEGER(Rind);

    Eigen::Map<Eigen::MatrixXd> tmpImage(image, dims[0], dims[1]);

    PointIndexConverter *conv;
    
     
    SingleGMRAData<double> *gmraData = dynamic_cast< SingleGMRAData<double>* >(data);
    if(gmraData!=NULL){
      GMRATree<double> *gmra = gmraData->getGMRA();
      int nPoints = gmra->getRoot()->getPoints().size();

      Eigen::Map<Eigen::MatrixXi> tmpInd(ind, 2, nPoints); 
      conv = new GMRAConverter<double>(tmpInd, gmraData );
      
    }
    else{
      Eigen::Map<Eigen::MatrixXi> tmpInd(ind, 2, data->getNumberOfPoints() );
      conv = new GeneralPurposeConverter(tmpInd);
    }


    ImageDEL<double> *del = new ImageDEL<double>(*data, tmpImage, conv);
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  *INTEGER(RshowBG) );
  
  };



  SEXP focusMultiIndexImageDEL(SEXP Rdata, SEXP Rimage, SEXP Rdims, SEXP Rind,
      SEXP RnInd, SEXP Rmulti, SEXP RnMulti, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh, SEXP RshowBG){  
    if(focus == NULL){
      return R_NilValue;
    }

    Data<double> *data = getData(Rdata);

    int *dims = INTEGER(Rdims);
    double *image = REAL(Rimage);  
    int *ind = INTEGER(Rind);
    int nInd = *INTEGER(RnInd);
    int *multi = INTEGER(Rmulti);
    int nMulti = *INTEGER(RnMulti);

    Eigen::Map<Eigen::MatrixXd> tmpImage(image, dims[0], dims[1]);

    PointIndexConverter *conv;
    
     
    Eigen::Map<Eigen::MatrixXi> tmpInd(ind, 2, nInd);
    Eigen::Map<Eigen::MatrixXi> multiInd(multi, nMulti, 2);
   
    int nPoints = multiInd.col(0).maxCoeff() + 1;
    
    std::vector< std::vector<int> > mInd(nPoints);
    for(int i=0; i< nMulti; i++){
      mInd[ multiInd(i, 0) ].push_back( multiInd(i, 1) );
    } 
    conv = new MultiSetGeneralPurposeConverter(tmpInd, mInd);
    
    //TODO not working with gmra data yet


    ImageDEL<double> *del = new ImageDEL<double>(*data, tmpImage, conv);
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  *INTEGER(RshowBG) );
  
  };
     


  SEXP focusMatrixDEL(SEXP Rdata, SEXP Rm, SEXP Rn, SEXP RcolumnMajor, SEXP
      Rtranspose, SEXP Raspect, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh){ 
    
    if(focus == NULL){
      return R_NilValue;
    }
    
    int m = *INTEGER(Rm);
    int n = *INTEGER(Rn);
    Data<double> *data = getData(Rdata);
    if(data->getDimension() !=  m*n ){
      Rprintf("m x n has to match data dimensionality");
      return R_NilValue;
    }
  
    
    MatrixDEL<double> *del=new MatrixDEL<double>(*data, m, n, *INTEGER(RcolumnMajor), *INTEGER(Rtranspose),
        *INTEGER(Raspect) ); 
    
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  true);
  };


  SEXP focusOffsetMatrixDEL(SEXP Rdata, SEXP Rm, SEXP Rn, SEXP RmAll, SEXP
      RnAll, SEXP Roffsets, SEXP RcolumnMajor, SEXP Rtranspose, SEXP Raspect,
      SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh){ 
    
    if(focus == NULL){
      return R_NilValue;
    }
    
    int m = *INTEGER(Rm);
    int n = *INTEGER(Rn);
    int mAll = *INTEGER(RmAll);
    int nAll = *INTEGER(RnAll);
    Data<double> *data = getData(Rdata);
    if(data->getDimension() !=  m*n ){
      Rprintf("m x n has to match data dimensionality");
      return R_NilValue;
    }
    Eigen::Map<Eigen::MatrixXi> tmp( INTEGER(Roffsets), 2, data->getNumberOfPoints() );
    Eigen::MatrixXi offsets = tmp; 
    
    OffsetMatrixDEL<double> *del=new OffsetMatrixDEL<double>(*data, m, n, mAll, nAll, offsets, *INTEGER(RcolumnMajor), *INTEGER(Rtranspose),
        *INTEGER(Raspect) ); 
    
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  true);
  };




  SEXP focusSubMatrixDEL(SEXP Rdata, SEXP Rm, SEXP Rn, SEXP Rindices, SEXP RcolumnMajor, SEXP
      Rtranspose, SEXP Raspect, SEXP Rx, SEXP Ry, SEXP Rw, SEXP Rh){ 
    
    if(focus == NULL){
      return R_NilValue;
    }
    
    int m = *INTEGER(Rm);
    int n = *INTEGER(Rn);
    int *indices = INTEGER(Rindices);
    std::vector<int> ind(m*n);
    for(int i=0; i<ind.size(); i++){
       ind[i] = indices[i]-1;
    }

    Data<double> *data = getData(Rdata);
    if(data->getDimension() !=  m*n ){
      Rprintf("m x n has to match data dimensionality");
      return R_NilValue;
    }
  
    
    SubMatrixDEL<double> *del=new SubMatrixDEL<double>(*data, m, n, ind, *INTEGER(RcolumnMajor), *INTEGER(Rtranspose), *INTEGER(Raspect) ); 
    
    return addDisplayElement(del, Rx, Ry, Rw, Rh,  true);
  };




 


  //----------------- Backgrounds for projection del
  
  ProjectionBackground<double> *getProjectionBackground(SEXP Rbg){
    SEXP Rbg_pointer = Rf_getAttrib(Rbg, Rf_install("focus_bg_ptr") );
    ProjectionBackground<double> *bg = static_cast<ProjectionBackground<double> *>( R_ExternalPtrAddr(Rbg_pointer) );
    return bg;
  };



  SEXP addProjectionBackground( ProjectionDEL<double> *del, ProjectionBackground<double> *bg){    
    
    pthread_mutex_lock(&mutex);
    
    del->addBackground(bg);
    
    SEXP Rid;
    PROTECT(Rid = Rf_allocVector(INTSXP, 1));
    INTEGER(Rid)[0] = bgCounter;
    SEXP ptr = R_MakeExternalPtr(bg, Rf_install("FOCUS"), R_NilValue);
    PROTECT(ptr);
    Rf_setAttrib(Rid, Rf_install("focus_bg_ptr"), ptr); 


    UNPROTECT(2);  

    ++bgCounter;

    updateFocus = true;
    pthread_mutex_unlock(&mutex);

    return Rid;
  };

 


  SEXP focusCircleBackground(SEXP Rpdel, SEXP Rradius, SEXP Rn){
    ProjectionDEL<double> *pdel = dynamic_cast< ProjectionDEL<double> * >( getDel(Rpdel) );
    if(pdel != NULL){
      CircleBackground<double> *bg = new CircleBackground<double>(*REAL(Rradius), *INTEGER(Rn) );
      return addProjectionBackground(pdel, bg); 
    }  
    return R_NilValue;
  };
  



  SEXP focusVerticalCircleLinesBackground(SEXP Rpdel, SEXP Rradius, SEXP Rn){
    ProjectionDEL<double> *pdel = dynamic_cast< ProjectionDEL<double> * >( getDel(Rpdel) );
    if(pdel != NULL){
      VerticalLinesCircleBackground<double> *bg
        = new VerticalLinesCircleBackground<double>(*REAL(Rradius), *INTEGER(Rn) );
      return addProjectionBackground(pdel, bg); 
    }
    return R_NilValue;
  };
  

  
  
  SEXP focusHorizontalCircleLinesBackground(SEXP Rpdel, SEXP Rradius, SEXP Rn){
    ProjectionDEL<double> *pdel = dynamic_cast< ProjectionDEL<double> * >( getDel(Rpdel) );
    if(pdel != NULL){
      HorizontalLinesCircleBackground<double> *bg = 
        new HorizontalLinesCircleBackground<double>(*REAL(Rradius), *INTEGER(Rn) );
      
      return addProjectionBackground(pdel, bg); 
    }
    return R_NilValue;
  };


  SEXP focusCenterShadowBackground(SEXP Rpdel, SEXP Rradius, SEXP Rn, SEXP Rscaled){
    ProjectionDEL<double> *pdel = dynamic_cast< ProjectionDEL<double> * >( getDel(Rpdel) );
    if(pdel != NULL){
      CenterBallShadowBackground<double> *bg 
        = new CenterBallShadowBackground<double>(*REAL(Rradius), *INTEGER(Rn),
            *INTEGER(Rscaled) ); 
      
      return addProjectionBackground(pdel, bg); 
    }
    return R_NilValue;
  };


  SEXP focusProjectorShadowBackground(SEXP Rpdel, SEXP Rradius, SEXP RrIndex, SEXP Rn, SEXP Rscaled){
    ProjectionDEL<double> *pdel = dynamic_cast< ProjectionDEL<double> * >( getDel(Rpdel) );
    if(pdel != NULL){
      ProjectorBallShadowBackground<double> *bg 
        = new ProjectorBallShadowBackground<double>(*REAL(Rradius),
            *INTEGER(RrIndex), *INTEGER(Rn), *INTEGER(Rscaled) );
      
      return addProjectionBackground(pdel, bg); 
    }
    return R_NilValue;
  };



  SEXP focusSetProjectionBackgroundColor(SEXP Rbg, SEXP Rr, SEXP Rg, SEXP Rb, SEXP Ra){

    ProjectionBackground<double> *bg = getProjectionBackground(Rbg);

    RGB col( *REAL(Rr), *REAL(Rg), *REAL(Rb) );
    bg->setColor(col, *REAL(Ra) );

    return R_NilValue;
  };



  


  //------------------ interaction calls


  SEXP focusClose(){
    stopFocus=true;
    return R_NilValue;
  };



  SEXP focusSetProjection(SEXP Rproj, SEXP Rv, SEXP Rmv, SEXP Rtype){
    if(focus == NULL){
      return R_NilValue;
    }

    Projector<double> *proj = getProjector(Rproj);

    double *v = REAL(Rv);
    int type = *INTEGER(Rtype);
    int mv = *INTEGER(Rmv); 
    
    if(mv == proj->getDimension() ){
      Eigen::Map< Eigen::VectorXd > p(v, mv);
      proj->setProjector(p, type); 
    }


    return R_NilValue;
  };





  SEXP focusSetActive(SEXP Rdata, SEXP Rv, SEXP Rmv){
    if(focus == NULL){
      return R_NilValue;
    }
    int *v = INTEGER(Rv);
    int mv = *INTEGER(Rmv); 

    std::vector<int> a(mv);
    for(int i=0; i<mv; i++){
      a[i] = v[i]-1;
    }
    Data<double> *data = getData(Rdata);
    data->setActive(a);

    return R_NilValue;
  };


  SEXP focusClearActive(SEXP Rdata){
    if(focus == NULL){
      return R_NilValue;
    }
    Data<double> *data = getData(Rdata);
    data->clearActive();

    return R_NilValue;
  }




  SEXP focusSetGroup(SEXP Rdata,SEXP Rv, SEXP Rmv, SEXP Rgid){
    if(focus == NULL){
      return R_NilValue;
    }
    int *v = INTEGER(Rv);
    int gid = *INTEGER(Rgid);
    int mv = *INTEGER(Rmv); 
    
    Data<double> *data = getData(Rdata);

    std::vector<int> g(mv);
    for(int i=0; i<mv; i++){
      g[i] = v[i]-1;
    }

    data->setGroup(gid, g);

    return R_NilValue;
  };


  SEXP focusGetGroup(SEXP Rdata, SEXP Rindices, SEXP Rn){
    if(focus == NULL){
      return R_NilValue;
    }
    
    Data<double> *data = getData(Rdata);
    
    int *indices = INTEGER(Rindices);
    int n = *INTEGER(Rn);

    int *g = new int[n];


    for(int i=0; i < n; i++){
      g[i] = data->getGroup( indices[i]-1 );
    }
    
    SEXP Rp;
    PROTECT( Rp = Rf_allocVector(INTSXP, n));
    memcpy( INTEGER(Rp), g, n*sizeof(int) );
    UNPROTECT(1);

    delete g;

    return Rp;
  };






  SEXP focusClearGroup(SEXP Rdata, SEXP Rgid){
    if(focus == NULL){
      return R_NilValue;
    }    
    
    Data<double> *data = getData(Rdata);
    int gid = *INTEGER(Rgid);

    data->clearGroup(gid);

    return R_NilValue;
  };




  SEXP focusAddPaired(SEXP Rdata, SEXP Rfrom, SEXP Rto, SEXP Rm){
    if(focus == NULL){
      return R_NilValue;
    }
    int *f = INTEGER(Rfrom);
    int *t = INTEGER(Rto);
    int m = *INTEGER(Rm); 


    Data<double> *data = getData(Rdata);
    
    std::vector<int> from(m);
    std::vector<int> to(m);

    for(int i=0; i<m; i++){
        from[i] = f[i]-1;
        to[i] = t[i]-1;
    }

    data->addPaired(from, to);

    return R_NilValue;
  };



  SEXP focusGetPaired(SEXP Rdata, SEXP Rfrom, SEXP Rn, SEXP Rgid){
    if(focus == NULL){
      return R_NilValue;
    }
    Data<double> *data = getData(Rdata);
    int *from = INTEGER(Rfrom);
    int n = *INTEGER(Rn);
    int gid = *INTEGER(Rgid); 

    std::vector<int> paired(n, -1);
    for(int i=0; i<n; i++){
      std::set<int> pairs = data->getPaired(from[i]-1);
      for(std::set<int>::iterator it = pairs.begin(); it != pairs.end(); ++it){
        int g = data->getGroup(*it);
        if(g == gid){
          paired[i] = *it;
          break;
        }
      }
    }

    SEXP Rp;
    PROTECT( Rp = Rf_allocVector(INTSXP, n));
    memcpy( INTEGER(Rp), paired.data(), n*sizeof(int) );
    UNPROTECT(1);

    return Rp;
  };





  SEXP focusClearPaired(SEXP Rdata){
    Data<double> *data = getData(Rdata);

    pthread_mutex_lock(&mutex);
    data->clearPaired();
    
    updateFocus = true;
    pthread_mutex_unlock(&mutex);

    return R_NilValue;
  };



  SEXP focusGetDensity(SEXP Rdata){
    Data<double> *data = getData(Rdata);

    std::vector<double> p(data->getNumberOfPoints() );
    for(int i=0; i<data->getNumberOfPoints(); i++){
      p[i] = data->getDensity(i);
    }
    SEXP Rp;
    PROTECT( Rp = Rf_allocVector(REALSXP, p.size()) );
    memcpy( REAL(Rp), p.data(), p.size()*sizeof(double) );
    UNPROTECT(1);


    return Rp;
  };


  SEXP focusSetDensity(SEXP Rdata, SEXP Rd, SEXP Rn){

    Data<double> *data = getData(Rdata);

    int n = *INTEGER(Rn);
    double *density = REAL(Rd);
    if(n != data->getNumberOfPoints() ){
      return R_NilValue;
    };

    std::vector< double > p( data->getNumberOfPoints() );
    for(unsigned int i=0; i<p.size(); i++){
      p[i] = density[i];
    }

    pthread_mutex_lock(&mutex);
    data->setDensity(p);

    updateFocus = true;
    pthread_mutex_unlock(&mutex);
    
    return R_NilValue;
  };



  SEXP focusExpandToScale(SEXP Rdata, SEXP Rscale){
    try{
      GMRAData<double> *data = dynamic_cast<GMRAData<double>*>( getData(Rdata) );
      int scale = *INTEGER(Rscale);
      pthread_mutex_lock(&mutex);
      data->expandToScale(scale);
       
      updateFocus = true;
      pthread_mutex_unlock(&mutex);
    }
    catch(...){
    }

    return R_NilValue;
  };



  SEXP focusSetBackgroundColor(SEXP Rr, SEXP Rg, SEXP Rb){

    RGB col( *REAL(Rr), *REAL(Rg), *REAL(Rb));
    if(focus != NULL){
      pthread_mutex_lock(&mutex);
      focus->setBackgroundColor(col);
      
      updateFocus = true;
      pthread_mutex_unlock(&mutex);
    }
    return R_NilValue;
  };




  SEXP focusSetDefaultColor(SEXP Rdata, SEXP Rr, SEXP Rg, SEXP Rb){

    Data<double> *data = getData(Rdata);

    RGB col( *REAL(Rr), *REAL(Rg), *REAL(Rb));
    pthread_mutex_lock(&mutex);
    data->setDefaultColor(col);

    updateFocus = true;
    pthread_mutex_unlock(&mutex);
    
    return R_NilValue;
  };



  SEXP focusSetGroupColor(SEXP Rdata, SEXP Rgid, SEXP Rr, SEXP Rg, SEXP Rb){

    Data<double> *data = getData(Rdata);

    RGB col( *REAL(Rr), *REAL(Rg), *REAL(Rb));
    pthread_mutex_lock(&mutex);
    
    data->setColor(*INTEGER(Rgid), col);

    updateFocus = true;
    pthread_mutex_unlock(&mutex);
    
    return R_NilValue;
  };



  

  SEXP focusAddLine(SEXP Rdata, SEXP Rline, SEXP Rm){
    
    int *line = INTEGER(Rline);
    int m = *INTEGER(Rm); 


    Data<double> *data = getData(Rdata);
    
    std::vector<int> l(m);

    for(int i=0; i<m; i++){
      l[i] = line[i]-1;
    }
    
    pthread_mutex_lock(&mutex);
    
    data->addLine(l);

    updateFocus = true;
    pthread_mutex_unlock(&mutex);

    return R_NilValue;
  };


  SEXP focusGetSelection(SEXP Rdata){
    Data<double> *data = getData(Rdata);

    std::vector<int> sel = data->getSelection();
    SEXP Rsel;
    PROTECT( Rsel = Rf_allocVector(INTSXP, sel.size()) );
    memcpy( INTEGER(Rsel), sel.data(), sel.size()*sizeof(int) );
    UNPROTECT(1);


    return Rsel;
  };











}//end extern C
