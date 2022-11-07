#ifndef LIGHTING_H
#define LIGHTING_H


#include "vector3d.h"
#include "visualObject.h"
class Shader;
class Lighting : public VisualObject
{
public:
    Lighting(Shader* shader);
    virtual void init() override;
    virtual void draw() override;

    //ambient light and colour
    GLfloat mAmbientStrength{ 0.1f };
    Vector3D mAbientColor{ 0.2f, 0.2f, 0.1f };

    //diffuse luight and colour
    GLfloat mLightStrenght{ 1.7f };
    Vector3D mLightColor{ 0.4f, 0.4f, 0.4f };    //Yellow

    //specular strenft and exponent

    GLfloat mSpecularStrenght{ 0.9f };
    GLint mSpecularExponent{ 4 };
};

#endif // LIGHTING_H
