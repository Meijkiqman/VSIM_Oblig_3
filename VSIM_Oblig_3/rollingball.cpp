#include "rollingball.h"
#include "surfacemesh.h"
RollingBall::RollingBall(std::string fileName, Shader* shader) : ObjMesh(fileName, shader)
{

}

void RollingBall::SetSurface(VisualObject* surface)
{
    m_Surface = surface;
}

void RollingBall::DoPhysics()
{
    qDebug() << "RollingBall: DoPhysics()";
    QVector3D gravity(0, -9.81f, 0);
    if(m_Surface){
        qDebug() << "Did Physics";
        Result r = dynamic_cast<SurfaceMesh*>(m_Surface)->GetHeight(GetPosition());
        Vertex* v1 = &r.v1;
        Vertex* v2 = &r.v2;
        Vertex* v3 = &r.v2;

        QVector3D currPos = GetPosition();

        float myWidth = 0.25;
        currPos.setY(r.height +(myWidth/2));
        SetPosition(currPos);
    }
}


