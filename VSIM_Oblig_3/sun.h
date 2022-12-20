#ifndef SUN_H
#define SUN_H


#include "objloader.h"

class sun : public objLoader
{
public:
    sun(std::string fileName, Shader* shader);
    bool SunPatrol = true;
    float SunPatrolDir = 0;
    float SunPos;
};

#endif // SUN_H
