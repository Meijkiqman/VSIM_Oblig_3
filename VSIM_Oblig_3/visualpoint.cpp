#include "visualpoint.h"

VisualPoint::VisualPoint(Shader* shader) : VisualObject(shader)
{

}

VisualPoint::VisualPoint(const std::vector<Vertex>& v, Shader *shader)  : VisualObject(shader)
{
    mVertices = v;
}

VisualPoint::~VisualPoint()
{

}

void VisualPoint::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<GLvoid*>(0));          // array buffer offset

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void VisualPoint::draw()
{
    initializeOpenGLFunctions();

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, mVertices.size());
}

std::vector<std::vector<float>> VisualPoint::createMatrix(const int& rows, const int& columns)
{
    return std::vector<std::vector<float>>(rows, std::vector<float>(columns));
}

std::vector<std::vector<float>> VisualPoint::multiplyMatrixes(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b)
{
    int aRows = a.size();
    int aColumns = a[0].size();
    int bRows = b.size();
    int bColumns = b.size();
    std::vector<std::vector<float>> newMatrix = createMatrix(bRows, aColumns);
    for (int i{}; i < aRows; ++i)
        for (int j{}; j < bColumns; ++j)
            for (int k{}; k < aColumns; ++k)
            {
                newMatrix[i][j] += a[i][k] * b[k][j];
            }
    return newMatrix;
}

std::vector<std::vector<float>> VisualPoint::transposeMatrix(const std::vector<std::vector<float>>& a)
{
    int aRows = a.size();
    int aColumns = a[0].size();
    std::vector<std::vector<float>> newMatrix = createMatrix(aColumns, aRows);

    for (int i{}; i < aColumns; ++i)
        for (int j{}; j < aRows; ++j)
        {
            newMatrix[i][j] = a[j][i];
        }
    return newMatrix;
}
