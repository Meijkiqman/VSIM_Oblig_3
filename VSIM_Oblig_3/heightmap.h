#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

//oppg2
#include "visualObject.h"
class heightMap : public VisualObject
{
public:
    heightMap(Shader* shader, Texture* texture);
    void init() override;
    void draw() override;
    unsigned int NUM_STRIPS = 0;
    unsigned int NUM_VERTS_PER_STRIP = 0;
    float GetHeight(QVector3D pos);
    QVector3D GetBaycentric(QVector3D point, QVector3D a, QVector3D b, QVector3D c);
    int width, height, nChannels;
    std::vector<float> vertices;
private:
    void readFile(std::string filePath);
};


#endif // HEIGHTMAP_H
