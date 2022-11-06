#include "soundmanager.h"
#include "wavfilereader.h"
#include <iostream>

SoundManager* SoundManager::mInstance = NULL;

SoundManager::SoundManager() :
    mDevice(NULL),
    mContext(NULL)
{ }

bool SoundManager::init()
{
    std::cout << "Intializing OpenAL!\n";
    mDevice = alcOpenDevice(NULL);
    if (mDevice)
    {
        mContext = alcCreateContext(mDevice, NULL);
        alcMakeContextCurrent(mContext);
    }

    // Generate buffers
    alGetError();

    if (!mDevice)
    {
        std::cout << "Device not made!\n";
    }
    else
        std::cout << "Intialization complete!\n";

    //Start listing of found sound devices:
    //Not jet implemented
    //ALDeviceList *pDeviceList = NULL;
    //ALCcontext *pContext = NULL;
    //ALCdevice *pDevice = NULL;
    //ALint i;	//will hold the number of the preferred device
    //ALboolean bReturn = AL_FALSE;

    return true;
}

void SoundManager::cleanUp()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

bool SoundManager::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        std::cout << "Invalid name!\n";
        return false;
    case AL_INVALID_ENUM:
        std::cout << "Invalid enum!\n";
        return false;
    case AL_INVALID_VALUE:
        std::cout << "Invalid value!\n";
        return false;
    case AL_INVALID_OPERATION:
        std::cout << "Invalid operation!\n";
        return false;
    case AL_OUT_OF_MEMORY:
        std::cout << "Out of memory!\n";
        return false;
    default: break;
    }
    return true;
}

SoundSource* SoundManager::createSource(std::string name, Vector3D pos, std::string filePath, bool loop, float gain)
{
    SoundSource* tempPtr = new SoundSource(name, loop, gain);
    tempPtr->setPosition(pos);
    if (filePath != "")
        tempPtr->loadWave(filePath);
    return tempPtr;
}

void SoundManager::updateListener(Vector3D pos, Vector3D vel, Vector3D dir, Vector3D up)
{
    //Have to convert Vector3 floats to ALfloats
    ALfloat posVec[3];
    ALfloat velVec[3];
    ALfloat headVec[6];
    posVec[0] = pos.mX;
    posVec[1] = pos.mY;
    posVec[2] = pos.mZ;
    velVec[0] = vel.mX;
    velVec[1] = vel.mY;
    velVec[2] = vel.mZ;
    headVec[0] = dir.mX;
    headVec[1] = dir.mY;
    headVec[2] = dir.mZ;
    headVec[3] = up.mX;
    headVec[4] = up.mY;
    headVec[5] = up.mZ;
    alListenerfv(AL_POSITION, posVec);
    alListenerfv(AL_VELOCITY, velVec);
    alListenerfv(AL_ORIENTATION, headVec);
}
