#ifndef ProfileDEL_H
#define ProfileDEL_H

#include "Display.h"
#include "Eigen/Dense"
#include "Rotation.h"
#include "Animator.h"
#include "Font.h"
#include "Projector.h"

#include <sstream>
#include <iomanip>

template<typename TPrecision>
class ProfileDEL : public AdjustableDisplayElement{

  private:

    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;

#define BUFSIZE 512



    int xM, yM;
    int pickH, pickW;  

    Data<TPrecision> &data;
    
    Font &font;
   
    int nAxis;
    int selected;
    bool drawCoordinates;
    bool drawText;

  public:
    ProfileDEL(Data<TPrecision> &d, Font &f) : data( d ), font(f){ 
      initStuff(); 
    };



    ~ProfileDEL(){
    };


    void init(){  

    };


    void displayAction(void){

      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
     
      glLineWidth(2);
           
     
      TPrecision a = data.getMin();
      TPrecision b = data.getMax();

      if(this->drawBackground() ){ 
        glColor4f(0.25, 0.25, 0.25, 1);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2f(xLeft, yTop + height * b / (b-a) );
        glVertex2f(xLeft + width, yTop + height * b / (b-a) );
        glEnd();

        if(drawCoordinates){ 
          //draw coordinates
          glColor4f(0.25, 0.25, 0.25, 1);
          glLineWidth(1);
          glBegin(GL_LINES);
          for(int i=0; i<nAxis; i++){
            double xc = xLeft + 10+ (i*(width-20))/(nAxis-1.0);
            glVertex2f(xc, yTop);
            glVertex2f(xc, yTop+height);
          }
          glEnd();
        }

        if(drawText){
          font.setSize(8);
          glColor4f(0.7, 0.7, 0.7, 1);
          for(int i=0; i<data.getDimension(); i++){
            double xc = xLeft + 10+ (i*(width-20))/(data.getDimension()-1.0);
            double yc = yTop+height;
            font.renderString(data.rowName(i), xc-2, yc, 0, true);
          }
        }

      }


      //static RGB gray(0.8, 0.8, 0.8);
      //drawProfile( projector.getProjector(0), a, b, gray, 2, 1, false);
      //drawProfile( projector.getProjector(1), a, b, gray, 2, 1, false, 0xAAAA, 2);

      //draw selected point
      int index = data.getSelected();
      if(index >= 0 && index < data.getNumberOfPoints() ){
        static RGB white(1,1,1);
        //drawProfile( data.getData(index), a, b, data.getColor(index), 2, 0.5, false);
      }
      
      //draw mean profile
      std::vector<int> &sel =data.getSelection();
      if(sel.size() > 0){
        VectorXp mean = data.getSelectionMean();
        drawProfile(mean, a, b, data.getColor(sel[0]), 4, 1, false);
        
        VectorXp lower = data.getSelectionLowerBound();
        drawProfile(lower, a, b, data.getColor(sel[0]), 2, 1, false, 0xAAAA, 1);

        VectorXp upper = data.getSelectionUpperBound();
        drawProfile(upper, a, b, data.getColor(sel[0]), 2, 1, false, 0xAAAA, 1);
      }


     if(isInside(xM, yM)){
        //draw pick rectangle
        glColor4f(0.75, 0.75, 0.75, 0.75);
        glLineWidth(2);
        double pw = pickW/2.0;
        double ph = pickH/2.0;
        glBegin(GL_LINE_LOOP);
        glVertex2f(xM-pw, yM-ph);
        glVertex2f(xM+pw, yM-ph);
        glVertex2f(xM+pw, yM+ph);
        glVertex2f(xM-pw, yM+ph);
        glEnd();
     }

     glPopName();
    };


    void idle(){

    };


    void keyboard(unsigned char key, int x, int y){
    };



    void special(int key, int x, int y){    
      pickW = 7;
      pickH = 7;

      selected = -1;

      nAxis = std::min(data.getDimension(), 50);
      drawCoordinates = true;//d.getDimension() <= 40;
      drawText = nAxis == data.getDimension();

      xM=-1;
      yM=-1;
    };




    void mouseAction(int button, int state, int x, int y){
    };



    void motionAction(int x, int y){
    };


    void passive(int x, int y){
    };






private:
   

    void initStuff(){
      pickW = 7;
      pickH = 7;

      selected = -1;

      nAxis = std::min(data.getDimension(), 50);
      drawCoordinates = true;//d.getDimension() <= 40;
      drawText = nAxis == data.getDimension();

      xM=-1;
      yM=-1;
    };

    
    
    void drawProfileScaled(const Eigen::Ref<const VectorXp> &v, RGB &col, int lwd = 1,
        double alpha=1, bool text=false, unsigned short pattern = 0xFFFF, int factor=1){

      TPrecision xMin = v.minCoeff();
      TPrecision xMax = v.maxCoeff();
      drawProfil(v, xMin, xMax, col, lwd, alpha, text, pattern, factor);

    };


    void drawProfile(const Eigen::Ref<const VectorXp> &v, TPrecision xMin,
        TPrecision xMax, RGB &col, int lwd = 1, double alpha=1, bool text=false,
        unsigned short pattern = 0xFFFF, int factor=1){

      TPrecision xD = xMax-xMin;
      
      int n= data.getDimension();

      if(text){
        glColor4f(0.5, 0.5, 0.5, 1);
        font.setSize(9);

        std::stringstream s1;
        s1 << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        s1 << xMin;
        font.renderString(s1.str(), xLeft-40, yTop+height);

        std::stringstream s2;
        s2 << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        s2 << xMax;
        font.renderString(s2.str(), xLeft-40, yTop+11);
      }


      glColor4f(col.r, col.g, col.b, alpha);

      glPushAttrib(GL_ENABLE_BIT); 

      glLineStipple(factor, pattern);  
      glEnable(GL_LINE_STIPPLE);
      glLineWidth(lwd);
      glBegin(GL_LINE_STRIP);
      for(int i=0; i<n; i++){
        double xc =  (i*(width-20))/(n-1.0)+xLeft+10;
        glVertex2f(xc, yTop + height - height*(v(i)-xMin)/xD);
      }
      glEnd();

      glPopAttrib();

    };


};

#endif
