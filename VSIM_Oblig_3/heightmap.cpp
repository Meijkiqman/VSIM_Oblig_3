#include "heightmap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "shader.h"
#include "vertex.h"
#include "texture.h"

heightMap::heightMap(Shader* shader,  Texture* texture) : VisualObject(shader)
{

    //array with pixels, relevant data: widht, height and hue. reads from file
    unsigned char* data = stbi_load("../VSIM-Oblig_3/Assets/EksamenHeightmap.png",
                                        &width, &height, &nChannels,
                                        0);
    //Taken from https://learnopengl.com/Guest-Articles/2021/Tessellation/Height-map
    //data contains width * height * hue of the gray, as nChannels elements
    //
    // vertex generation
    float yScale = 64.0f / 256.0f, yShift = 32.0f;  // apply a scale+shift to the height data

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            //X vertex starts at the very right, then adds i to what posistion along the x graph
            vertices.push_back(-height / 2.0f + i);        // v.x
            //mvoes along with the x, then by y
            vertices.push_back((int)y * yScale - yShift); // v.y
            //Y does the same as x
            vertices.push_back(-width / 2.0f + j);        // v.z
        }
    }

    // The y values are now read and the image can be removed from memory
    stbi_image_free(data);

    //Create mesh from these vertices
    for (long int i = 0; i < vertices.size(); i += 3)
    {
        mVertices.push_back(Vertex(vertices[i], vertices[i + 1], vertices[i + 2]));
    }
    // index generation
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < height - 1; i++)       // for each row a.k.a. each strip
    {
        for (unsigned int j = 0; j < width; j++)      // for each column
        {
            for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                mIndices.push_back(j + width * (i + k));
            }
        }
    }
    NUM_STRIPS = height - 1;
    NUM_VERTS_PER_STRIP = width * 2;


    mTexture= texture;
    mMatrix.setToIdentity();

}

void heightMap::init()
{
    initializeOpenGLFunctions();
    //Get the model matrix from shader
    mMatrixUniform = glGetUniformLocation(mShader->getProgram(), "mMatrix");
    //Vertex array object-VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex buffer object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    //Verts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Element array buffer - EAB
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void heightMap::draw()
{
    //texute
    if (mTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
        glUniform1i(mTextureUniform, 0);
    }
    //shader
    glUseProgram(mShader->getProgram());
    //model matrix
    mShader->SetUniformMatrix4fv(mMatrix, "mMatrix");
    //Draws object
    glBindVertexArray(mVAO);
    for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * NUM_VERTS_PER_STRIP * strip));
    }
    glBindVertexArray(0);
}



Result heightMap::GetHeight(QVector3D pos)
{
    Result r;
    float a = 0, b = 0, c = 0, height = 0;
    //Sjekker for trekant 1
    QVector3D bary =  GetBarycentric(pos, mVertices[0], mVertices[1], mVertices[2]);
    if(((0 <= bary.x()) && (bary.x() <= 1)) && ((0 <= bary.y()) && (bary.y() <= 1)) && ((0 <= bary.z()) && (bary.z() <= 1))){
        //Er denne trekanten
        a = mVertices[0].y * bary.x();
        b = mVertices[1].y * bary.y();
        c = mVertices[2].y * bary.z();
        height = a + b + c;
        r.height = height;
        r.v1 = mVertices[0];
        r.v2 = mVertices[1];
        r.v3 = mVertices[2];
        qDebug() << "Returned triangle 1";
        return r;
    }
    bary = GetBarycentric(pos, mVertices[1], mVertices[3], mVertices[2]);
    if(((0 <= bary.x()) && (bary.x() <= 1)) && ((0 <= bary.y()) && (bary.y() <= 1)) && ((0 <= bary.z()) && (bary.z() <= 1))){
        //Er denne trekanten
        a = mVertices[1].y * bary.x();
        b = mVertices[3].y * bary.y();
        c = mVertices[2].y * bary.z();
        height = a + b + c;
        r.height = height;
        r.v1 = mVertices[1];
        r.v2 = mVertices[3];
        r.v3 = mVertices[2];
        qDebug() << "Returned triangle 2";
        return r;
    }
    bary = GetBarycentric(pos, mVertices[1], mVertices[5], mVertices[3]);
    if(((0 <= bary.x()) && (bary.x() <= 1)) && ((0 <= bary.y()) && (bary.y() <= 1)) && ((0 <= bary.z()) && (bary.z() <= 1))){
        //Er denne trekanten
        a = mVertices[1].y * bary.x();
        b = mVertices[5].y * bary.y();
        c = mVertices[3].y * bary.z();
        height = a + b + c;
        r.height = height;
        r.v1 = mVertices[1];
        r.v2 = mVertices[5];
        r.v3 = mVertices[3];
        qDebug() << "Returned triangle 3";
        return r;
    }
    bary = GetBarycentric(pos, mVertices[1], mVertices[4], mVertices[5]);
    if(((0 <= bary.x()) && (bary.x() <= 1)) && ((0 <= bary.y()) && (bary.y() <= 1)) && ((0 <= bary.z()) && (bary.z() <= 1))){
        //Er denne trekanten
        a = mVertices[1].y * bary.x();
        b = mVertices[4].y * bary.y();
        c = mVertices[5].y * bary.z();
        height = a + b + c;
        r.height = height;
        r.v1 = mVertices[1];
        r.v2 = mVertices[4];
        r.v3 = mVertices[5];
        qDebug() << "Returned triangle 4";
        return r;
    }
    return r;
}




QVector3D heightMap::GetBarycentric(QVector3D point, Vertex v1,  Vertex v2, Vertex v3){
    double u = 0, v = 0, w = 0;
    QVector3D a(v1.x,v1.y,v1.z);
    QVector3D b(v2.x,v2.y,v2.z);
    QVector3D c(v3.x,v3.y,v3.z);
    //Finner x
    QVector3D x1 = b - a;
    QVector3D x2 = c - a;
    x1.setY(0);
    x2.setY(0);
    QVector3D normal = QVector3D::crossProduct(x1, x2);
    float x = normal.length();
    //Regner kryssprodukt av p og q, og deler på x
    //Bruker a - point, b - point og c - point og kryssproduktet av kombinasjonen av de
    QVector3D p = b - point;
    QVector3D q = c - point;
    p.setY(0);
    q.setY(0);
    //Kryss produkt for u, delt på x
    normal = QVector3D::crossProduct(p,q);
    u = normal.y() / x;

    p = c - point;
    q = a - point;
    p.setY(0);
    q.setY(0);
    //Kryss produkt for v, delt på x
    normal = QVector3D::crossProduct(p, q);
    v = normal.y() / x;

    p = a - point;
    q = b - point;
    p.setY(0);
    q.setY(0);
    //Kryssprodukt for w, delt på x
    normal = QVector3D::crossProduct(p, q);
    w = normal.y() / x;
    //qDebug() << u << v << w;
    //Finn om punkten er innenfor eller utenfor triangelt
    return QVector3D(u, v, w);

}
