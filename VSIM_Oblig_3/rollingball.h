#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "objmesh.h"

class RollingBall : public ObjMesh
{
public:
    RollingBall(std::string fileName, Shader* shader);

    void SetSurface(VisualObject* surface);
    VisualObject* GetSurface(){return m_Surface;};
    void DoPhysics() override;

private:
    VisualObject* m_Surface;
    float m_Weight = 0.2f;
    QVector3D m_Velocity;
    QVector3D mAcceleration;
    float m_GravityConstant = 9.81f;
};

#endif // ROLLINGBALL_H
