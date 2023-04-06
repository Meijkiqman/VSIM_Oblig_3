#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "vertex.h"
#include "visualObject.h"

class Vertex;
struct Result
{
    float height;
    Vertex v1;
    Vertex v2;
    Vertex v3;
public:
    void setV1(const Vertex &newV1);
};
class heightMap : public VisualObject
{
public:
    heightMap(Shader* shader, Texture* texture);
    void init() override;
    void draw() override;
    unsigned int NUM_STRIPS = 0;
    unsigned int NUM_VERTS_PER_STRIP = 0;
    //barycentric stuff



    Result GetHeight(QVector3D pos);
    QVector3D GetBarycentric(QVector3D point, Vertex v1,  Vertex v2, Vertex v3);
    int width, height, nChannels;
    std::vector<float> vertices;
private:
    void readFile(std::string filePath);
};


#endif // HEIGHTMAP_H
