#ifndef BEZIER_H
#define BEZIER_H


#include "visualobject.h"
#include <vector>
class Bezier : public VisualObject
{
public:
    Bezier(std::vector<QVector3D> controlPoints, Shader* shader);
    void init(GLint matrixUniform);
    void draw() override;
    int d = 3;
public:
     QVector3D EvaluateBezier(float t);
private:
    std::vector<QVector3D> mControlPoints;
    std::vector<Vertex> mControlPointsVertices;

};


#endif // BEZIER_H
