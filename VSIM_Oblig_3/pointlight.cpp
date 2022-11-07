#include "pointlight.h"
#include "shader.h"


PointLight::PointLight(Shader* shader, int pointLightIndex) : Lighting(shader)
{
    mPointLightIndex = pointLightIndex;
    mMatrix.setToIdentity();
}

PointLight::PointLight(Shader* shader, Texture* texture) : Lighting(shader)
{
    mTexture = texture;
    mMatrix.setToIdentity();
}

PointLight::PointLight(Shader* shader, Texture* texture, int pointLightIndex) : Lighting(shader)
{
    mPointLightIndex = pointLightIndex;
    mTexture = texture;
    mMatrix.setToIdentity();
}

void PointLight::init()
{
    Lighting::init();
}

void PointLight::draw()
{
    mShader->use();
    mShader->SetUniform3f(mPosition.column(3).x(), mPosition.column(3).y(), mPosition.column(3).z(),
        "pointLights[" + std::to_string(mPointLightIndex) + "].position");
    //Give it ambient
    mShader->SetUniform3f(mLightColor.mX * mAmbientStrength, mLightColor.mY * mAmbientStrength, mLightColor.mZ * mAmbientStrength,
        "pointLights[" + std::to_string(mPointLightIndex) + "].ambient");
    //Diffuse
    mShader->SetUniform3f(mLightColor.mX, mLightColor.mY, mLightColor.mZ,
        "pointLights[" + std::to_string(mPointLightIndex) + "].diffuse");
    //Maybe specular, should maybe be per object
    mShader->SetUniform3f(1.f, 1.f, 1.f, "pointLights[" + std::to_string(mPointLightIndex) + "].specular");
    //Constant
    mShader->SetUniform1f(1.0f, "pointLights[" + std::to_string(mPointLightIndex) + "].constant");
    //Linear
    mShader->SetUniform1f(0.029f, "pointLights[" + std::to_string(mPointLightIndex) + "].linear");
    //Quadtratic
    mShader->SetUniform1f(0.032f, "pointLights[" + std::to_string(mPointLightIndex) + "].quadtratic");
    Lighting::draw();
}
