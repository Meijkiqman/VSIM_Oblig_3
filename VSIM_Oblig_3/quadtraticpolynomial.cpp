#include "quadtraticpolynomial.h"

QuadtraticPolynomial::QuadtraticPolynomial(float a, float b, float c, float dx)
{
    for (auto x = -10; x <= 10; x += 1)
    {
        auto y = p(a, b, c, x);
        mVertices.push_back(Vertex(x, y, 0, 0, 1, 0));
    }
    mMatrix.setToIdentity();

}

double QuadtraticPolynomial::p(float a, float b, float c, float x)
{
    return a * x * x + b * x + c;
}
float QuadtraticPolynomial::GetYPos(float x)
{
    return p(ma, mb, mc, x);
}
