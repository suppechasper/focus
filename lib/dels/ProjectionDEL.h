#ifndef PROJECTIONDEL_H
#define PROJECTIONDEL_H


#include <vector>

#include "Eigen/Dense"

#include "GMRAData.h"

#include "Projector.h"
#include "AdjustableDisplayElement.h"

#include "Font.h"

//
template< typename TPrecision> class ProjectionBackground;



template<typename TPrecision>
class ProjectionDEL : public AdjustableDisplayElement{

  protected:
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, Eigen::Dynamic> MatrixXp;
    typedef typename Eigen::Matrix<TPrecision, Eigen::Dynamic, 1> VectorXp;


#define BUFSIZE 512

    MatrixXp X;

    Data<TPrecision> &data;
    Projector<TPrecision> &projector;

    Font &font;

    bool drawPoints;
    bool drawLines;
    bool passiveSelect;

    bool drawIdentifiers;
    bool drawScaleFactor;

    int size; 

    int xM, yM;
    int pickH, pickW;  



    float alpha;
    float pointSize;

    std::vector< ProjectionBackground<TPrecision> *> backgrounds;



  public:

    ProjectionDEL(Projector<TPrecision> &p, Font &f, bool dPoints=true, bool dLines = false, bool passive=true) 
      : data( p.getData() ), projector(p), font(f), drawPoints(dPoints),
      drawLines(dLines), passiveSelect(passive){ 

        pickW = 5;
        pickH = 5;
        alpha = 0.1;
        pointSize = 6;
        xM = -1;
        yM=-1;
        size = 0;

        drawIdentifiers = false;
        drawScaleFactor = false;
        
      };


    virtual ~ProjectionDEL(){
    };


    Projector<TPrecision> &getProjector(){
      return projector;
    };

    virtual void reshape(int wi, int he){
      AdjustableDisplayElement::reshape(wi, he);
      size = std::min(width, height)/2 - 5;
    };




    virtual void addBackground(ProjectionBackground<TPrecision> *bg){
      backgrounds.push_back(bg);
    };



    virtual void displayBackground(){
      for(int i=0; i<backgrounds.size(); i++){
        backgrounds[i]->drawBackground( this, &projector);
      }
    };

    virtual void displayAction(void){

      using namespace Eigen;


      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();

      if(  this->drawBackground() ){ 
        displayBackground();
      }

      MatrixXp &X = projector.getProjected();
      MatrixXp &P = projector.getProjectedProjectors();

      if(drawPoints){
        glPointSize(this->pointSize);
        glBegin(GL_POINTS);
        for(int i=0; i< X.cols(); i++){
          const VectorXp &p= X.col(i);
          VectorXp x = toScreen(p);

          double a;
          if(this->data.isActive(i) ){
            a = 0.1 + 0.9 * this->alpha * this->data.getDensity(i);
          }
          else{
            a = this->alpha * 0.1 * this->data.getDensity(i);
          }



          RGB &col = this->data.getColor(i);
          glColor4f(col.r, col.g, col.b, a);
          glVertex2f( x(0), x(1) );

        }
        glEnd();
      }



      if(drawLines){
        glLineWidth(this->pointSize);

        std::vector< std::vector<int> > &lines = data.getLines();

        for(int k=0; k<lines.size(); k++){
          std::vector<int> &strip = lines[k];

          glBegin(GL_LINE_STRIP);
          for(int i=0; i< strip.size(); i++){
            const VectorXp &p= X.col( strip[i] );
            VectorXp x = toScreen(p);

            double a =0.1 + 0.9 * this->alpha * this->data.getDensity( strip[i] );
            RGB &col = this->data.getColor( strip[i] );
            glColor4f( col.r, col.g, col.b, a );
            glVertex2f( x(0), x(1) );
          }

          glEnd();
        }
      }


      this->font.setSize(11);
      int index= this->data.getSelected();
      if(index >= 0 && index < X.cols() ){
        const VectorXp &p = X.col( index );
        VectorXp x = toScreen(p);

        {
          RGB &col = this->data.getColor(index);
          glColor4f(col.r, col.g, col.b, 1);
          this->font.setSize(12); 
          this->font.renderString(this->data.colName(index), x(0), x(1));
        }

        double a;
        if(this->data.isActive(index) ){
          a = 0.5 + 0.5*this->alpha;
        }else{
          a = 0.25;
        }

        RGB &col = this->data.getDefaultColor();
        glColor4f(col.r, col.g, col.b, a);
        
        std::set< int > &paired = this->data.getPaired( index);
        for(std::set<int>::iterator it = paired.begin(); it != paired.end(); ++it){
          int j= *it;
          const VectorXp &p = X.col( j );
          VectorXp y = toScreen(p);

          glBegin(GL_LINES);


          glVertex2f(x(0), x(1));
          glVertex2f(y(0), y(1));

          glEnd();

        }
      }

      //draw projector identifiers
      if(drawIdentifiers){
        glPointSize(8);
        glColor4f(1, 1, 1, 1); 
        this->font.setSize(8); 
        for(int i=0; i<P.cols(); i++){
          const VectorXp &p = P.col(i);
          VectorXp x = toScreen(p);

          std::stringstream s;
          s << i; 
          this->font.renderString(s.str(), x(0), x(1));
          glBegin(GL_POINTS);
          glVertex2f(x(0), x(1));
          glEnd();

        }
      }

      //draw projection scale factor  
      if(drawScaleFactor){
        std::stringstream s;
        this->font.setSize(12); 
        s << std::setprecision(2) << projector.getScaleFactor(); 
        VectorXp scalePos = toScreen(-1, -1);
        this->font.renderString(s.str(), scalePos(0)+20, scalePos(1)+8 );
        this->font.setSize(12); 


      }


      if(this->isInside(this->xM, this->yM)){
        //draw pick rectangle
        static RGB col(0.75, 0.75, 0.75);
        drawCircle(this->xM, this->yM, this->pickW, col, 2, 50);
      }

    };





    virtual void keyboard(unsigned char key, int x, int y){
      //std::vector<int> keep;
      if(!isInside(x, y)){ return; };
      switch(key)
      {

        case 'c':
        case 'C':
          data.clearSelection(); 
          break;
        case '+':    
          pickW+=1;
          pickH+=1;
          break;
        case '-':
          pickW-=1;
          pickH-=1;
          if(pickW < 5){
            pickW=5;
          }
          if(pickH < 5){
            pickH=5;
          }
          break;
        case '>':{
          TPrecision scaleF = projector.getScaleFactor();
          projector.setScaleFactor(scaleF*1.1);
          break;
                 }
        case '<':{
          TPrecision scaleF = projector.getScaleFactor();
          projector.setScaleFactor(scaleF*0.9);
          break;
                 }

      }
      glutPostRedisplay();
    };



    virtual void special(int key, int x, int y){
      switch(key)
      {
        case GLUT_KEY_LEFT:
          alpha *= 0.95;
          break;
        case GLUT_KEY_RIGHT:
          alpha *= 1.05;
          break;
        case GLUT_KEY_DOWN:
          pointSize *= 0.95;
          break;
        case GLUT_KEY_UP:
          pointSize *= 1.05;
          break;

      }
      if(alpha < 0){
        alpha=0;
      }
      else if(alpha > 1){
        // alpha=1;
      }

      glutPostRedisplay();
    };






    virtual void mouseAction(int button, int state, int x, int y){
      xM = x;
      yM = y;
      if(!isInside(x, y)){ return; };

      int mod = glutGetModifiers();
      
      if(!passiveSelect && state == GLUT_DOWN){
        selectDataPoints(x, y, mod == GLUT_ACTIVE_SHIFT) ;
      }

      int index = data.getSelected();
      if (state == GLUT_DOWN && index >= 0 && index < data.getNumberOfPoints() ){

        if(button == GLUT_LEFT_BUTTON ){
          VectorXp mean = data.getSelectionMean();
          if(mean.squaredNorm() == 0 ){
            mean = data.getData( index );
          }

          if(mod == GLUT_ACTIVE_CTRL ){    
            projector.setProjector(mean, 0);        
          }
          else if(mod == GLUT_ACTIVE_SHIFT ){
            projector.setProjector(mean, 1);        
          }
          else{
            projector.setProjector(mean, 2);        
          }

        }
        else if(button == GLUT_RIGHT_BUTTON){
          if(mod == GLUT_ACTIVE_SHIFT ){            
            try{
              GMRAData<TPrecision> &tmp = dynamic_cast<GMRAData<TPrecision>& >( data); 
              std::vector<int> &sel = data.getSelection();
              tmp.expandAll( sel );
            }
            catch(...){
            }
          }
          else if(mod == GLUT_ACTIVE_CTRL ){            
            try{
              GMRAData<TPrecision> &tmp = dynamic_cast<GMRAData<TPrecision>& >( data); 
              std::vector<int> sel = data.getSelection();
              tmp.collapseNodes( sel );

            }
            catch(...){
            }
          }

          else if( mod == GLUT_ACTIVE_ALT ){            
            try{
              GMRAData<TPrecision> &tmp = dynamic_cast<GMRAData<TPrecision>& >( data); 
              tmp.deleteNode(index);
            }
            catch(...){
            }
            glutPostRedisplay();

          }
          else{
            try{
              GMRAData<TPrecision> &tmp = dynamic_cast<GMRAData<TPrecision>& >( data);
              std::vector<int> sel = data.getSelection();
              tmp.expandNodes( sel );
            }
            catch(...){
            }

          }

          glutPostRedisplay();
        }
      }

    };









    virtual void passive(int x, int y){
      xM = x;
      yM = y;


      if(!isInside(x, y)){ 
        return; 
      };    

      if(passiveSelect){
        selectDataPoints(x, y);
        glutPostRedisplay();
      }



    };


    virtual void motionAction(int x, int y){};

    virtual void idle(){};


    virtual void init(){  };


    virtual VectorXp toScreen(const  VectorXp  &x) {
      return toScreen( x(0), x(1) );
    };


    virtual VectorXp toData(const Eigen::Ref< Eigen::VectorXi > &x) {
      return toData( x(0), x(1) );
    };


    virtual VectorXp toScreen(TPrecision x, TPrecision y){
      VectorXp v(2);

      v(0) = this->xLeft + (x+1.0) * this->size;
      v(1) = this->yTop  + (y+1.0) * this->size; 
      return v;
    };


    virtual VectorXp toData(int x, int y){
      VectorXp v(2);
      v(0) = (x - this->xLeft ) / (double) this->size - 1.0;
      v(1) = (y - this->yTop)   / (double) this->size - 1.0;
      return v;
    };





  private:

    void drawCircle( TPrecision x, TPrecision y, TPrecision r, RGB &col, int lwd=1, int n=50 ){
      glLineWidth(lwd);

      glColor4f(col.r, col.g, col.b, 0.75);

      glBegin(GL_LINE_LOOP);
      for(int i =0; i <= n; i++){
        double angle = 2 * M_PI * i / n;
        TPrecision x1 = x + cos(angle)*r;
        TPrecision y1 = y + sin(angle)*r ;
        glVertex2d( x1, y1 );
      }
      glEnd();

    };


    void selectDataPoints(int x, int y, bool add = false){

      VectorXp xd = toData(x, y);
      double wd = pickW/(double)size;
      double hd = pickH/(double)size;

      if(!add){
        data.clearSelection();
      }

      MatrixXp &X = projector.getProjected();
      for(int i=0; i< X.cols(); i++){
        const VectorXp &p = X.col(i);
        if(fabs(p(0) - xd(0)) < wd && fabs(p(1) - xd(1) ) < hd){
          data.addSelected(i);
        }
      }
      
      std::vector<int> &select = data.getSelection();
      if(select.size() > 0 ){
        data.setSelected( *select.rbegin() );
      }
      else{
        data.setSelected(-1);
      }

    };

};

#endif
