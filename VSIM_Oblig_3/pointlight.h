#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "lighting.h"
class PointLight : public Lighting
{
public:
    PointLight(Shader* shader, int pointLightIndex);
    PointLight(Shader* shader, Texture* texture);
    PointLight(Shader* shader, Texture* texture, int pointLightIndex);
    virtual void init() override;
    virtual void draw() override;

    int mPointLightIndex = 0;
};

#endif // POINTLIGHT_H
