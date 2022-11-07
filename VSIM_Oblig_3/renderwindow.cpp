#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>
#include <iostream>
#include <chrono>   //for sleep_for
#include <thread>

#include "shader.h"
#include "objLoader.h"
#include "camera.h"
#include "texture.h"
#include "visualobject.h"
#include "soundmanager.h"
#include "vector3d.h"
#include "soundsource.h"
#include "lighting.h"
#include "pointlight.h"
#include "mainwindow.h"
#include "logger.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

   // //This is the matrix used to transform (rotate) the triangle
   // //You could do without, but then you have to simplify the shader and shader setup
   // mMVPmatrix = new QMatrix4x4{};
   // mMVPmatrix->setToIdentity();    //1, 1, 1, 1 in the diagonal of the matrix

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}


// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT


    mShaders.insert(std::pair<std::string, Shader*>{"PlainShader", new Shader("../VSIM_Oblig_3/shaders/plainshader.vert",
        "../VSIM_Oblig_3/shaders/plainshader.frag")});

    mShaders.insert(std::pair<std::string, Shader*>{"phongshader", new Shader("../VSIM_Oblig_3/shaders/phongshader.vert",
        "../VSIM_Oblig_3/shaders/phongshader.frag")});

    mShaders.insert(std::pair<std::string, Shader*>{"TextureShader", new Shader("../VSIM_Oblig_3/shaders/textureshader.vert",
        "../VSIM_Oblig_3/shaders/textureshader.frag")});

   

    //Set the material properties of the lightshader
   mShaders["phongshader"]->use();
   mShaders["phongshader"]->SetUniform1i(0, "material.diffuse");
   mShaders["phongshader"]->SetUniform1i(1, "material.specular");
   mShaders["phongshader"]->SetUniform1f(32.0f, "material.shininess");

   mShaders["phongshader"]->SetUniform3f(0.2f, 1.0f, 0.3f, "dirLight.direction");
   mShaders["phongshader"]->SetUniform3f(0.1f, 0.1f, 0.1f, "dirLight.ambient");
   mShaders["phongshader"]->SetUniform3f(0.4f, 0.4f, 0.4f, "dirLight.diffuse");
   mShaders["phongshader"]->SetUniform3f(0.5f, 0.5f, 0.5f, "dirLight.specular");

    

    //creates camera
    mCamera = new Camera();


    //mMap insert of primitve objects:
    mMap.insert(std::pair<std::string, VisualObject*>{"plane",
        new objLoader("../VSIM_Oblig_3/objFiles/plane.obj", mShaders["phongshader"], new Texture("..//VSIM_Oblig_3/Textures/chessboard.bmp"))});

    mMap.insert(std::pair<std::string, VisualObject*>{"cube",
        new objLoader("../VSIM_Oblig_3/objFiles/cube.obj", mShaders["phongshader"], new Texture("..//VSIM_Oblig_3/Textures/sivert.bmp"))});

    mMap.insert(std::pair<std::string, VisualObject*>{"ball",
                  new objLoader("../VSIM_Oblig_3//objFiles/ball.obj", mShaders["phongshader"])});


    mMap["plane"]->SetPosition(QVector3D(0, -5, 0));
    mMap["plane"]->SetScale(QVector3D(10, 10, 10));

    mMap["cube"]->SetPosition(QVector3D(50, 0, 0));
    mMap["cube"]->SetScale(QVector3D(5, 5, 5));

    //lights
     mMap.insert(std::pair<std::string, VisualObject*>{"pointLight ", new PointLight(mShaders["phongshader"], new Texture())});

     for (int i = 0; i < 10; i++)
      {
          //Construct point light with light shader and with its index, so it call set it correctly in the draw function
          mMap.insert(std::pair<std::string, VisualObject*>{"PointLight " + std::to_string(i), new PointLight(mShaders["phongshader"], new Texture(), mPointLights)});
          mPointLights++;
      }
        //Set the phongshader.frag's pointLightUsed to the amount of point lights
        mShaders["phongshader"]->SetUniform1i(mPointLights, "pointLightsUsed");


    SoundManager::getInstance()->init();

    //placing the listener:
    Vector3D pos(0.0f, 0.0f, 0.0f);  //at world origo
    Vector3D vel(0.0f, 0.0f, 0.0f);  //no velocity
    Vector3D dir(0.0f, 0.0f, -1.0f); //direction into the screen (openal uses right hand axis)
    Vector3D up(0.0f, 1.0f, 0.0f);   //Y is up

    //set the listener position
    SoundManager::getInstance()->updateListener(pos, vel, dir, up);

    mExplosionSound = SoundManager::getInstance()->createSource(
                   "Explosion", Vector3D(10.0f, 0.0f, 0.0f),
                   "../VSIM_Oblig_3/Assets/explosion.wav", false, 1.0f);
       mLaserSound = SoundManager::getInstance()->createSource(
                   "Laser", Vector3D(0.0f, 0.0f, 0.0f),
                   "../VSIM_Oblig_3/Assets/laser.wav", true, 0.4f);

       mStereoSound = SoundManager::getInstance()->createSource(
                   "Stereo", Vector3D(),
                   "../VSIM_Oblig_3/Assets/stereo.wav", false, 1.0f);

       mSong = SoundManager::getInstance()->createSource(
                   "Caravan", Vector3D(),
                   "../VSIM_Oblig_3/Assets/Caravan_mono.wav", false, 1.0f);

     //init every object
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        (*it).second->init();
        (*it).second->UpdateTransform();
        //qDebug() << "object init";
    }
    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    //qDebug() << "render start";
    calculateFramerate(); //display framerate
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    mCamera->init();
    // verticalAngle, aspectRatio, nearPlane,farPlane
    mCamera->perspective(90, static_cast<float>(width()) / static_cast<float>(height()), 0.1, 3000.0);
    mCamera->lookAt(camPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    QVector3D ballPos = mMap["ball"]->GetPosition();
    Vector3D soundPos = mMap["ball"]->getPosition();
    //Vector3D postest = mMap["ball"]->getPosition();



     mLaserSound->setPosition(soundPos);


    for (auto it = mPointLightsMap.begin(); it != mPointLightsMap.end(); it++)
       {
           QVector3D dir = QVector3D(-1 + rand() % 2, -1 + rand() % 2, -1 + rand() % 2 );
           QVector3D pos = (*it).second->GetPosition();
           (*it).second->SetPosition((pos + dir) * 0.05f);

       }

    //qDebug() << "camera created";

    //Apply camera to all shaders
    for (auto it = mShaders.begin(); it != mShaders.end(); it++) {
        (*it).second->use();
        //Send view and projection matrices to alle the shaders
        (*it).second->SetUniformMatrix4fv(*mCamera->mVmatrix, "vMatrix");
        (*it).second->SetUniformMatrix4fv(*mCamera->mPmatrix, "pMatrix");
        //glUnifor
        //The visual object sends its own modelMatrix to the shader so it dosent need to be done here
        if ((*it).first == "phongshader")
        {
            //Give all lights the camera position
            (*it).second->SetUniform3f(mCamera->GetPosition().x(), mCamera->GetPosition().y(), mCamera->GetPosition().y(),
                "cameraPosition");
        }
    }



    //Draw all objects
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
           //Set the shader matrixes from camera
           (*it).second->UpdateTransform();
           (*it).second->draw();
            // qDebug() << "object draw finished";
    }
  

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }
    if(event->key() == Qt::Key_W)
    {
           mMap["ball"]->move(0,0,-5);
     }

     if(event->key() == Qt::Key_S)
     {
         mMap["ball"]->move(0,0,5);
     }

     if(event->key() == Qt::Key_A)
     {
         mMap["ball"]->move(-5,0,0);
     }

     if(event->key() == Qt::Key_D)
     {
         mMap["ball"]->move(5,0,0);
     }

     if(event->key() == Qt::Key_Q){
         mMap["ball"]->move(0,5,0);
     }

     if(event->key() == Qt::Key_E)
     {
         mMap["ball"]->move(0,-5,0);
     }
     if(event->key() == Qt::Key_G)
     {
         //1. Stereo sounds can not be moved ******************************
             std::cout << "\nPlaying stereo wav file - positioning have no effect\n";
             mStereoSound->play();
     }
     if(event->key() == Qt::Key_H)
     {
         std::cout << "\nPlaying looping Laser sound\n";
         mLaserSound->play();
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
         // mLaserSound->stop();

     }


  // QVector3D temp = camPos;
  // //camLookAt = QVector3D(0, 0, 0);
  // if (event->key() == Qt::Key_W)
  // {

  //     camPos.setZ(temp.z() + 5);
  //    // camLookAt.setZ(temp.z() + 5);
  //     qDebug() << "camera pos:" << camPos;
  // }

  // if (event->key() == Qt::Key_S)
  // {
  //     camPos.setZ(temp.z()  - 5);
  //     //camLookAt.setZ(temp.z()  - 5);
  //     qDebug() << "camera pos:" << camPos;
  // }

  // if (event->key() == Qt::Key_A)
  // {
  //     camPos.setX(temp.x() - 5);
  //    // camLookAt.setX(temp.x() - 5);
  //     qDebug() << "camera pos:" << camPos;
  // }
  //

  // if (event->key() == Qt::Key_D)
  // {
  //     camPos.setX(temp.x() + 5);
  //    // camLookAt.setX(temp.x() + 5);
  //     qDebug() << "camera pos:" << camPos;
  // }

  // if (event->key() == Qt::Key_Q)
  // {
  //     camPos.setY(temp.y() + 10);
  //    // camLookAt.setY(temp.y() + 20);
  //     qDebug() << "camera pos:" << camPos;
  // }

  // if (event->key() == Qt::Key_E)
  // {
  //     camPos.setY(temp.y() - 10);
  //     // camLookAt.setY(temp.y() - 20);
  //     qDebug() << "camera pos:" << camPos;
  // }

    //You get the keyboard input like this
//    if(event->key() == Qt::Key_A)
//    {
//        mMainWindow->statusBar()->showMessage(" AAAA");
//    }
//    if(event->key() == Qt::Key_S)
//    {
//        mMainWindow->statusBar()->showMessage(" SSSS");
//    }
}
