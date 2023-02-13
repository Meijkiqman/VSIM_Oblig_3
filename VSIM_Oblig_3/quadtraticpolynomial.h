#ifndef QUADTRATICPOLYNOMIAL_H
#define QUADTRATICPOLYNOMIAL_H
#include "visualcurve.h"

class QuadtraticPolynomial : public VisualCurve
{
public:
    QuadtraticPolynomial(float a, float b, float c, float dx);
    float GetYPos(float x);

private:
    float ma, mb, mc;
    double p(float a, float b, float c, float x);
};
#endif // QUADTRATICPOLYNOIAL_H
