
#include "Eigen/Dense"
#include "EigenLinalgIO.h"


#include "StandardData.h"
#include "SparseData.h"
#include "StandardDataProxy.h"
#include "GMRAData.h"
#include "Focus.h"

#include "GroupSVDObserver.h"
#include "AllSVDObserver.h"

//Projectors
#include "OrthogonalProjector.h"
#include "HyperbolicOrthogonalProjector.h"
#include "LinearProjector.h"
#include "CorrelationProjector.h"

#include "SinglePointFocusProjector.h"
#include "HyperbolicSinglePointFocusProjector.h"

#include "TwoPointFocusProjector.h"
#include "HyperbolicTwoPointFocusProjector.h"

#include "ThreePointFocusProjector.h"

#include "LineFocusProjector.h"
#include "HyperbolicLineFocusProjector.h"

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


#include <tclap/CmdLine.h>

#include "GlutStrokeFont.h"




#define MAKE_STRING_(x) #x
#define MAKE_STRING(x) MAKE_STRING_(x)


Focus<double> *focus;

void display1(void){
  focus->display();
};


void idle1(void){
  focus->idle();
};

void mouse1(int button, int state, int x, int y){
  focus->mouse(button, state, x, y);
}

void motion1(int x, int y){
  focus->motion(x, y);
}

void passive1(int x, int y){
  focus->passive(x, y);
}

void keyboard1(unsigned char key, int x, int y){
  focus->keyboard(key, x, y);
}

void special1(int key, int x, int y){
  focus->special(key, x, y);
}

void reshape1(int w, int h){
  focus->reshape(w, h);
}



void visible(int vis)
{
    if (vis == GLUT_VISIBLE)
        glutIdleFunc(idle1);
    else
        glutIdleFunc(NULL);
}

void printHelp(){
  //std::cout << focus->title() << " Window" << std::endl << std::endl;
  focus->printHelp();	
  //std::cout <<  std::endl << std::endl;
}



int main(int argc, char **argv){	
  
  using namespace EigenLinalg;
  using namespace Eigen;
  
  //Command line parsing
  TCLAP::CmdLine cmd("Focus", ' ', "1");
        
  TCLAP::ValueArg<std::string>  mArg("m", "matrix", "Data matrix (double)", true, "", "");
  cmd.add(mArg);  
 
  
  try{
    cmd.parse( argc, argv );
  } 
  catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
    return -1;
  }


  GlutStrokeFont font;
   
  MatrixXd m= LinalgIO<double>::readMatrix(mArg.getValue());



  SparseData<double> spData;

  StandardData<double> data(m);
  StandardDataProxy<double> dataProxy(m, data);
  //data.setRowNames(rn);
  //data.setColNames(cn);
  RGB col(1, 0.7, 0);
  data.setDefaultColor( col );

  //GL stuff
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);


  //Windows
  focus = new Focus<double>(font);
  Animator &animator = focus->getAnimator();
  
  //Setup windows
  double xpc = 0.2;
  double wpr=0.5;
  double hpr=0.8;


  OrthogonalProjector<double> ortho( data, true, false, true );
  HyperbolicOrthogonalProjector<double> hortho( data, true, false, false );

  LinearProjector<double> linear( data, true, true );
  CorrelationProjector<double> corr( data );
  
  
  SinglePointFocusProjector<double> spf( data, true, false, false );
  HyperbolicSinglePointFocusProjector<double> hspf( data, true, false, false );
  
  TwoPointFocusProjector<double> twpf( data, true );
  HyperbolicTwoPointFocusProjector<double> htwpf( data, true );
  
  LineFocusProjector<double> line( data );
  HyperbolicLineFocusProjector<double> hline( data );
  
  ThreePointFocusProjector<double> thpf( data );

  HeatTriangulationProjector<double> htp( data );

  //Annimate projectors
  RotationProjectorAnimator<double> pRotator(animator);
  TangentRotationProjectorAnimator<double> tpRotator(animator);
  ortho.setProjectorAnimator(&pRotator);

  //group projectors
  GroupedProjectorObserver<double> group;
  group.addProjector(&ortho);
  group.addProjector(&hortho);

  ortho.addObserver(&group);
  hortho.addObserver(&group);

  ProjectionDEL<double> *pd = 
    new ProjectionDEL<double>(hspf, font);
  pd->location(xpc, 0.2, wpr, hpr);
  
  
  focus->addDisplayElement(pd);



  FocusColorProjectorObserver<double> focusColor(0); 
  hspf.addObserver(&focusColor);

  CircleBackground<double> *cbg = new CircleBackground<double>(1.0);
  pd->addBackground(cbg);



  CenterBallShadowBackground<double> *cbbg = new CenterBallShadowBackground<double>(0.5);
  pd->addBackground(cbbg);

  ProfileDEL<double> *pr=new ProfileDEL<double>(data,font);
  pr->location(xpc, 0, 1-xpc, 0.2);
  focus->addDisplayElement(pr);

  PointsDEL<double> *pts = new PointsDEL<double>(data, font);

  int nGroups = 9;
  double a = 1.0/(nGroups+2);

  AllSVDObserver<double> allSVD;
  allSVD.dataChanged(&data, DataObserver<double>::DATA);
  data.addObserver(&allSVD);

  PCADEL<double> *b1 = new PCADEL<double>(&allSVD, ortho, data.getDefaultColor() );
  b1->location(0, 0, xpc, a); 
  focus->addDisplayElement(b1);

  for(int i=0; i < nGroups; i++){     
    RGB &col = data.getGroupColor(i);
    
    GroupSVDObserver<double> *gSVD = new GroupSVDObserver<double>(i);
    gSVD->dataChanged(&data, DataObserver<double>::DATA);
    data.addObserver(gSVD);
    
    PCADEL<double> *b = new PCADEL<double>(gSVD, ortho, col);
    b->location(0, (i+1)*a, xpc, a); 
    focus->addDisplayElement(b);
  }
 

  //run glut 
  glutInitWindowSize(1100, 710); 
  int mainWindow = glutCreateWindow(focus->title().c_str());
  glutDisplayFunc(display1);
  glutVisibilityFunc(visible);
  glutReshapeFunc(reshape1);
  glutMouseFunc(mouse1);
  glutMotionFunc(motion1);
  glutPassiveMotionFunc(passive1);
  glutKeyboardFunc(keyboard1);
  glutSpecialFunc(special1);
  
  focus->init();

  printHelp();


  glutMainLoop();

  return 0;
}


