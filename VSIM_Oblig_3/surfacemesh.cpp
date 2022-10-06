#include "surfacemesh.h"
#include "vertex.h"
SurfaceMesh::SurfaceMesh(Shader* s) : VisualObject(s)
{
    //leser gjennom las txt fila for hver linje

    //henter txt fila
   std::ifstream file("../VSIM_Oblig_3/LAS/test.txt");
   std::vector<float> points;

   if(file.is_open())
   {
       //current line
       std::string line;
       //finner mellomrom, finner sise space for å hente tall
       int lastSpace = 0;
       //lagerer stringen for å konvertere det til flloat eller double
       std::string  number;
       while(std::getline(file, line) )
       {
           //setter lastspace til 0 slik at man kan telle antall mellomrom
           lastSpace = 0;
           //går gjennom hver linje
           for(int i = 0; i< line.size(); i++)
           {
               //finner space
               if(line[i] == ' ')
               {
                   number = line.substr(lastSpace, i-1);
                   //konverter fra string til double
                   points.push_back(std::stod(number));
                   lastSpace = i;

               }
               else if(i == line.size() -1)
               {
                   number = line.substr(lastSpace, i);
                   //konverter fra string til float
                   points.push_back((std::stof(number)));
               }
           }
       }

   }
   file.close();

   if(false)
   {
       float lowestX  =150;
       float lowestY  =150;
       float lowestZ  =150;
       float highestX =150;
       float highestY =150;
       float highestZ =150;
       for(int i = 0; i < points.size(); i+=3)
       {
           if(points[i] > highestX)
           {
               highestX = points[i];
           }
           if(points[i] < lowestX)
           {
               lowestX = points[i];
           }
           if(points[i+1] > highestY)
           {
               highestY = points[i+1];
           }
           if(points[i+1] < lowestY)
           {
               lowestY = points[i+1];
           }
           if(points[i+2] > highestZ)
           {
               highestZ = points[i+2];
           }
           if(points[i+2] < lowestZ)
           {
               lowestZ = points[i+2];
           }
       }
       qDebug() << "lowest x value: " << lowestX;
       qDebug() << "lowest y value: " << lowestY;
       qDebug() << "lowest z value: " << lowestZ;
       qDebug() << "highest x value: " << highestX;
       qDebug() << "highest y value: " << highestY;
       qDebug() << "highest z value: " << highestZ;
   }

   //tallene ble veldig høye og punktene langt fra hverandre,deler på 100, 100000 0g 1000000 for å få de lavere
   for(int i =0;i < points.size(); i++)
   {

        //deler x på 100 for å få ned til 100 plass
        if(points[i] >476800 && points[i] < 576800 )
        {
            points[i] /=1000000;
        }
        //deler på 1000 for å få ned til 100 plass
        else if(points[i] > 5.6762e+06 &&points[i] < 6.6762e+06)
        {
            points[i] /=10000000;
        }
          //ganger med 10 for å få det til 1000 plass
        else if(points[i] > 0.019637 &&points[i] < 382.56)
        {
            points[i] /=1000;
        }

    }

    for(int i = 0; i < points.size(); i+=10)
    {
        mVertices.push_back(Vertex(points[i], points[i + 2], points[i + 1], sin(i), cos(i), tan(i)));
    }

    for (int i = 0; i < 30; i+=3)
    {
         qDebug() << points[i] << " " << points[i +2] << " " << points[i+1];
    }

   //Lager convex hull
   std::vector<Quad> mQuads;
   for (float i = 0; i < height; i+=res)
   {
       for (float j = 0; j < width; j+=res)
       {
           //Top right, top left, bottom right, bottom left
           mQuads.push_back(Quad{ j, j+res, i, i+res });
       }
   }

   QVector3D pos;
   float index = 0;
   for(int i = 0; i < points.size(); i+=200)
   {
           pos = {points[i], points[i+1], points[i+2]};
           if(pos.x() > width || pos.y() > height)
           {}
           else
           {
               index = (pos.x()/res) + (trunc((width/res)-1) * trunc(pos.y()/res));
               index = trunc((pos.x()/res)) + (trunc((width/res)-1) * trunc(pos.y()/res));
               index = trunc(index);
               if(index < mQuads.size())
               {
                   mQuads[index].addHeight(pos.z());
               }
             }
   }
   //Lager vertexer
   for (int i = 0; i < mQuads.size(); i++)
   {
       //Lager vertexer
       mVertices.push_back(Vertex(mQuads[i].GetCenter().x, mQuads[i].GetHeight(), mQuads[i].GetCenter().y, sin(i), cos(i), tan(i)));
   }

  for(int j = 0; j < (height/res)-1; j++)
  {
      for(int i = 0; i < (width/res)-1 ; i++)
      {
          //Første index
          mIndices.push_back(i+((width/res)*j));
          ////Høyre for første
          mIndices.push_back(i+((width/res)*j)+1);
          ////Under første
          mIndices.push_back(i+((width/res)*j)+width/res);
          //Høyre for første
          mIndices.push_back(i+((width/res)*j)+1);
          //Under første
          mIndices.push_back(i+((width/res)*j)+width/res);
          //Til høyre for under første
          mIndices.push_back(i+((width/res)*j)+width/res+1);
      }
  }
   mQuads.clear();

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

    //aktiverer merkering av vertices, punkter
    //glPointSize(2.0f);
    //glDrawArrays(GL_POINTS, 0, mVertices.size());
    //glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

Result SurfaceMesh::GetHeight(QVector3D pos)
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
        r.friction = 0.2;
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
        r.friction = 0.2;
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
        r.friction = 0.4;
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
        r.friction = 0.2;
        return r;
    }
    return r;
}

QVector3D SurfaceMesh::GetBarycentric(QVector3D point, Vertex v1,  Vertex v2, Vertex v3){
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




