#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "visualObject.h"
class objLoader : public VisualObject
{
public:
    objLoader(std::string filename);
    objLoader(std::string fileName, Shader* shader);
    objLoader(std::string fileName, Shader* shader, Texture* texture);
    virtual void draw() override;
    virtual void init() override;

protected:
    void readFile(std::string filePath);
    void writeFile(std::string filePath);
    GLuint mIBO{ 0 };
};

#endif // OBJLOADER_H
