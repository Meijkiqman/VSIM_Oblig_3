#ifndef SURFACEMESH_H
#define SURFACEMESH_H

#include "visualobject.h"

class SurfaceMesh : public VisualObject
{
public:
    SurfaceMesh(Shader* s);

    float GetHeight(QVector3D pos);

    void draw() override;
    void init() override;
};

#endif // SURFACEMESH_H
