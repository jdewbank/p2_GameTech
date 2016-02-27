/*
-----------------------------------------------------------------------------
Filename:    Application.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "Application.h"

//---------------------------------------------------------------------------
Application::Application(void)
{
}
//---------------------------------------------------------------------------
Application::~Application(void)
{
}

//---------------------------------------------------------------------------
void Application::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    //Set up Physics
    mPhysics = new PhysicsWorld();

    //Lights
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(0, 50, 200);

    mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

    //Camera
    mCamera->setPosition(0, 0, 350);

    //Sphere
    mBall = new Ball(mSceneMgr, mPhysics);


    Ogre::Real fieldSize = 200;
    //Paddle
    Ogre::Vector3 paddleSpecs = Ogre::Vector3(.4f,.3f,.1f);
    mPaddle = new Paddle(mSceneMgr, paddleSpecs, fieldSize, mPhysics);


    //PlayingField
    
    mField = new PlayingField(mSceneMgr, Ogre::Vector3(fieldSize,fieldSize,fieldSize), mPhysics);
}


bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool super = BaseApplication::frameRenderingQueued(evt);

    if(super)
    {
        if(mPhysics == NULL)
            return super;

        mPhysics->stepSimulation(evt.timeSinceLastFrame);
        if (mKeyboard->isKeyDown(OIS::KC_W)) 
        {
            //std::cout << "Move up" << std::endl;
            movementCommands[0] = 1;
        } else movementCommands[0] = 0;
        if (mKeyboard->isKeyDown(OIS::KC_A))
        {
            //std::cout << "Move left" << std::endl;
            movementCommands[1] = 1;
        } else movementCommands[1] = 0;
        if (mKeyboard->isKeyDown(OIS::KC_S))
        {
            //std::cout << "Move right" << std::endl;
            movementCommands[2] = 1;
        } else movementCommands[2] = 0;
        if (mKeyboard->isKeyDown(OIS::KC_D))
        {
            //std::cout << "Move down" << std::endl;
            movementCommands[3] = 1;
        } else movementCommands[3] = 0;
        mPhysics->move(movementCommands, evt.timeSinceLastFrame);

    }

    return super;
}


//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Application app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
