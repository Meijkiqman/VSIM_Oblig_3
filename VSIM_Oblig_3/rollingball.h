#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "objmesh.h"

class Vertex;
class RollingBall : public ObjMesh
{
public:
    RollingBall(std::string fileName, Shader* shader);

    void SetSurface(VisualObject* surface);
    void draw() override;
    VisualObject* GetSurface(){return m_Surface;};
    void DoPhysics() override;
    void ResetPhysics();
    void AddLife();
    bool Alive()
    {
        return timeLived <= totalLifeTime ? true : false;
    }

    //funksjoner for å aktivire/deaktivere splines
    void EnableSpline();
    void DisableSpline();

    int GettimeLived()
    {
        return timeLived;
    };

    int GetTotalLifeTime()
    {
        return totalLifeTime;
    };

    class visualPoint* GetSpline()
    {
        return mBspline;
    };

private:
    VisualObject* m_Surface;
    class SurfaceMesh* m_SurfaceMesh;


    //levetid for regn
    int timeLived = 0;
    int totalLifeTime = 600;
    //barysentriske koordinater erklæringer:
    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D a;
    QVector3D b;
    QVector3D normal;
    Vertex* oldv1;
    Vertex* oldv2;
    Vertex* oldv3;
    QVector3D oldNormal;
    QVector3D oldVel = QVector3D(0,0,0);
    QVector3D oldPos;
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    QVector3D m_Velocity;
    QVector3D mAcceleration;
    float m_Weight = 0.2f;

    float m_GravityConstant = 9.81f;

    float radius = 0.575f;

    //B-spline ting:
    int BsplinePointCounter = 60;
    void CreateBsplineP();
    std::vector<Vertex> ControlPs;
    void CreateBspline();

    class visualPoint* mControlPointVisual;
    class visualPoint* mBspline;

    bool drawSpline = false;


    //skjøtepunkt
    std::vector<float> t;

    std::vector<QVector3D> c;


    int findKnotInterval(float x);

    int n;
    //grad
    int d;
    //deBoors
    QVector3D EvaluateBezier(float x);



};

#endif // ROLLINGBALL_H
