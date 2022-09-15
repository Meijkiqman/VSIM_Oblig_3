#include "surfacemesh.h"
#include "vertex.h"
SurfaceMesh::SurfaceMesh(Shader* s) : VisualObject(s)
{
    //Vertex'er
    Vertex v1(-2.4f, 0.45f, 2.f,    1.f, 0.3f,0.5f);
    Vertex v2(0.f, 0.f, 2.f,        0.3f, 1.f,0.2f);
    Vertex v3(-2.5f, 0.f, -2.f,     0.5f, 0.6f,1.f);

    Vertex v4(0.f, .5f, -2.f,       1.f, 0.4f,0.2f);
    Vertex v5(3.0f, 0.1f, 1.925f,   0.3f, 1.f,0.3f);
    Vertex v6(3.0f, 0.f, -1.9f,     0.1f, 0.2f,2.f);

    //1
    mVertices.push_back(v1);
    mVertices.push_back(v2);
    mVertices.push_back(v3);

    mVertices.push_back(v4);
    mVertices.push_back(v5);
    mVertices.push_back(v6);



    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);

    mIndices.push_back(1);
    mIndices.push_back(3);
    mIndices.push_back(2);

    mIndices.push_back(1);
    mIndices.push_back(5);
    mIndices.push_back(3);

    mIndices.push_back(1);
    mIndices.push_back(4);
    mIndices.push_back(5);


}
void SurfaceMesh::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}
void SurfaceMesh::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);

    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
float SurfaceMesh::GetHeight(QVector3D pos)
{
    return 0;
}



