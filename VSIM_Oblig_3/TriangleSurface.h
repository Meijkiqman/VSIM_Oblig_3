#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();

    TriangleSurface(std::string filnavn);
      ~TriangleSurface();
      //Read the file and fill the mVertices
      void readFile(std::string fileName);
      //Write mVertices to the file
      void writeFile(std::string fileName);


      virtual void init() override;
      virtual void draw() override;

      void construct();
      static float func(float x, float y){
          //Matte oblig funksjon
          return sin(PI*x) * sin(PI*y);
      }
      //matte oblig 2
      static float func2(float x)
      {
          return 0.174 * x + 1, 731;
      }
      void CalculateNumerical();
};

#endif // TRIANGLESURFACE_H
