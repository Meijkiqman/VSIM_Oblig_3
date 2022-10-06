#include "rollingball.h"
#include "surfacemesh.h"
#include "visualpoint.h"
#include "shader.h"
RollingBall::RollingBall(std::string fileName, Shader* shader) : ObjMesh(fileName, shader)
{
    SetScale(QVector3D(5,5,5));
    oldPos = GetPosition();

}

void RollingBall::SetSurface(VisualObject* surface)
{
    m_Surface = surface;

    m_SurfaceMesh = dynamic_cast<SurfaceMesh*>(m_Surface);
}
void RollingBall::draw()
{
    //use my shader
    glUseProgram(mShader->getProgram());
    //Send my model matrix
    mShader->SetUniformMatrix4fv(mMatrix, "mMatrix");
    //Draw object
    glBindVertexArray( mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    if(drawSpline && mBspline)
    {
        mBspline->draw();
        if(mControlPointVisual)
        {
           mControlPointVisual->draw();
        }
    }

}
void RollingBall::DoPhysics()
{
    //3d vector gravtisjon med 9.81 retning nedover.
    QVector3D gravity(0, -9.81f, 0);
    //Radius finner man ved å finne størreksen på legemet og dele på 2
    radius = GetScale().x()/2;
    if(m_SurfaceMesh)
    {
        //Får resultatet fra surfacemesh
        Result r = m_SurfaceMesh->GetHeight(GetPosition());
        //om ballen er u luften, vil den falle i fritt fall.
        if(GetPosition().y() > r.height + 5)
        {
            SetPosition(GetPosition() + oldVel/60 + 1/2*gravity/60);
            oldVel = oldVel + gravity/60;
            oldPos = GetPosition();
            return;
        }
        //Lagerer vertexene på triangelt
        v1 = &r.v1;
        v2 = &r.v2;
        v3 = &r.v3;
        //Nåværende posistjon
        QVector3D currPos = GetPosition();
        //Sjekk at vertex finnes
        if(v1 && v2 && v3){
            //Lager punkter fra vertexene
            p1 = QVector3D(v1->x, v1->y, v1->z);
            p2 = QVector3D(v2->x, v2->y, v2->z);
            p3 = QVector3D(v3->x, v3->y, v3->z);
            //Finner normalen
            a = p2 - p1;
            b = p3 - p1;
            normal = QVector3D::crossProduct(a, b).normalized();
            //Akselerasjon, Lignign 7
            QVector3D acc = -gravity.y() * QVector3D(normal.x() * normal.y(), normal.y() * normal.z(), pow(normal.y(), 2)-1);
            //R = r0 + v0t + 1/2at^2, delt på 60 for dt = 1/60, 60 fps
            QVector3D nyPos = currPos + oldVel/60 + 1/2*acc/60;
            //Setter høyde + radius så den sitter på flaten
            nyPos.setY(r.height + radius);
            //Setter nye posisjonen
            SetPosition(nyPos);
            //Oppdater hastigheten
            oldVel = oldVel + acc /60;

            //SJekk om ballen er på ny trekant
            if(oldv1 && oldv2 && oldv3 && oldv1 != v1 &&oldv2 != v2 &&oldv3 != v3)
            {
                if(normal != oldNormal)
                {
                     //Ballen har rullet over på nytt triangel
                     //Beregner normalen til kolisjonsplanet, ligning 9
                     //Lage punkter fra de gamle vertexene
                     //Regne normalen mellom planene
                     //Finner en vector mellom de to veggene
                     QVector3D x = (normal + oldNormal)/((normal+oldNormal).lengthSquared());
                     //Formel 8.8 skal finne den nye hastighet vektoren
                     QVector3D nyHastighet = oldVel - 2*(oldVel*x)*x;
                     oldVel = nyHastighet;
                }
            }
            oldv1 = &r.v1;
            oldv2 = &r.v2;
            oldv3 = &r.v3;
            oldNormal = normal;
            oldPos = GetPosition();
        }

    }
    else
    {
        //Bare tyngdekraften påvirker ballen
        SetPosition(GetPosition() + oldVel/60 + 1/2*gravity/60 + QVector3D(sin(timeLived), 0 , sin(timeLived)));
        oldVel = oldVel + gravity /60;
        oldPos = GetPosition();
        return;
    }
}
void RollingBall::ResetPhysics()
{
    oldVel = QVector3D(0,0,0);
}

void RollingBall::AddLife()
{
    //adds lifetime for creating points
    timeLived += 1;
    if(timeLived % BsplinePointCounter ==0)
    {
        CreateBsplineP();
    }
}

void RollingBall::EnableSpline()
{
    qDebug() << "Enable spline called";
    if(!drawSpline)
    {
        CreateBspline();
        drawSpline = true;
    }
}

void RollingBall::DisableSpline()
{
     drawSpline = false;
}

void RollingBall::CreateBsplineP()
{
    QVector3D ballPos = GetPosition();
    Vertex* v = new Vertex(ballPos.x(), ballPos.y(), ballPos.z());
    //qDebug() << "Creating control point at " << ballPos;
    //qDebug() << "Vertexen er  på : " <<  QVector3D(v->x, v->y, v->z);
    ControlPs.push_back(*v);
}

void RollingBall::CreateBspline()
{
    qDebug() << " CREATE BSPLINE CALLED";

    //clears controlspoints
    t.clear();
    float step = 0.01;
    //-1 pga 0 indeksering
    n = ControlPs.size();
    //d=2, for kvadratisk spline
    d = 2;
    //Skjøter i hver ende
       float s = d + 1; //d + 1 like skjøter i hver ende
       float total = n + d + 1;
       //Lager skjøtevektoren
       for(int i = 0; i < total-s; i++)
       {
           if(i == 0)
           {
               for(int k = 0 ; k < s ; k++)
               {
                   t.push_back(i);
               }
           }
           else if(i == total-s-1){
               for(int k = 0; k < s; k++)
               {
                   t.push_back(i);
               }
           }
           else
           {
               t.push_back(i);
           }
       }

    for(int i = 0; i < ControlPs.size(); i++)
    {
           c.push_back(QVector3D(ControlPs[i].x, ControlPs[i].y,ControlPs[i].z));
    }

    std::vector<Vertex> mVisualPoints;
    for(float time = 0; time < 1; time += step)
    {
        QVector3D point = EvaluateBezier(time);
        qDebug() << "Bezier function returned : " << point << " for time " << time;

        mVisualPoints.push_back(Vertex(point.x(), point.y(), point.z(), tan(time),cos(time)));
    }

    mBspline = new visualPoint(mVisualPoints);

    mControlPointVisual = new visualPoint(ControlPs);
    mControlPointVisual->init();
    mBspline->init();
    drawSpline = true;


}
//deBoors fra compendium
QVector3D  RollingBall::EvaluateBezier(float x)
{
    int my = findKnotInterval(x);

    std::vector<QVector3D> a;

    a.reserve(d+1);

    for( int j =0; j<=d ; j++)
    {
        a[d-j]= c[my-j];
    }
    for(int k=d ; k>0; k--)
    {
        int j = my-k;
        for ( int i =0; i<k ; i++)
        {
            j ++;
            float w = (x-t[j] ) / ( t[j+k]-t[j] ) ;
            a[i] = a[i] * (1-w) + a [i +1] * w;
        }
    }
    return a[0] ;
}
//fra compendium
int RollingBall::findKnotInterval(float x)
{
    int my = n;
    while(x < t[my])
    {
        my--;
    }
    return my;
}

