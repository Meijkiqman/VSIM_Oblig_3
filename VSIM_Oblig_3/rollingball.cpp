#include "rollingball.h"

RollingBall::RollingBall(Shader* shader) : ObjMesh( "../VSIM_Oblig_3/ball.obj", shader)
{

}

void RollingBall::SetSurface(VisualObject* surface)
{
    m_Surface = surface;
}

void RollingBall::DoPhysics()
{
    if(m_Surface){
        QVector3D gravity(0, -9.81f, 0);
    }
}
