#ifndef SURFACEMESH_H
#define SURFACEMESH_H

#include "vertex.h"
#include "visualobject.h"
#include <iostream>
#include <string>

class Vertex;
struct Result{
    float height;
    Vertex v1;
    Vertex v2;
    Vertex v3;
    float friction = 0;

public:
    void setV1(const Vertex &newV1);
};


//for indeksering av flate
struct Point
{
    double x;
    double y;

    void SetXY(double a, double b)
    {
        x = a;
        y = b;
    }
};

struct Quad
{
    Point topRight;
    Point topLeft;
    Point bottomRight;
    Point bottomLeft;

    float height = 1;
    int amount = 0;
    void addHeight(float h)
    {
        height += h;
        amount++;
    }
    void SetHeight(float h)
    {
           height = h;
    }
    float GetHeight()
    {
        if(amount == 0 || height == 0)
        {
            return 15;
        }
        return height / amount;
    }
    Point GetCenter()
    {
        return Point{ (topRight.x + topLeft.x) / 2, (topRight.y + bottomLeft.y) / 2};
    }

};


class SurfaceMesh : public VisualObject
{
public:
    SurfaceMesh(Shader* s);
    void draw() override;
    void init() override;
    //
    QVector3D GetBarycentric(QVector3D point, Vertex v1,  Vertex v2, Vertex v3);
    //
    Result GetHeight(QVector3D pos);




    float width = 200;
    float height = 200;
    float res = 1;

    //verdier og ærklaringer for barysentriske
    //First v
    Vertex v1;
    //2nd v
    Vertex v2;
    //3rd v
    Vertex v3;
    //4th v
    Vertex v4;
    //5th v
    Vertex v5;
    //6th v
    Vertex v6;

    Vertex inUseVertex1;
    Vertex inUseVertex2;
    Vertex inUseVertex3;

    QVector3D a;
    QVector3D b;
    QVector3D c;

    QVector3D x1;
    QVector3D x2;

    QVector3D normal;

    QVector3D p;
    QVector3D q;


};

#endif // SURFACEMESH_H
