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
    light->setPosition(0, -50, -200);

    //Shadows
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    //Camera
    mCamera->setPosition(0, -350, -200);
    mCamera->lookAt(0, 0, 0);

    //Sphere
    mBall = new Ball(mSceneMgr, mPhysics);

    Ogre::Real fieldSize = 200;

    //Paddle
    Ogre::Vector3 paddleSpecs = Ogre::Vector3(.4f,.3f,.1f);
    mPaddle = new Paddle(mSceneMgr, paddleSpecs, fieldSize, mPhysics);

    //PlayingField    
    mField = new PlayingField(mSceneMgr, 
        Ogre::Vector3(fieldSize,fieldSize,fieldSize), mPhysics);

    //Sounds
    mSound = new SoundPlayer(); 
    mPhysics->setSoundPlayer(mSound);
}


bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool super = BaseApplication::frameRenderingQueued(evt);

    if(super)
    {
        if(mPhysics == NULL)
            return super;

        //TRANSLATION
        movementCommands[0] = mKeyboard->isKeyDown(OIS::KC_W)? 1:0; // up 
        movementCommands[1] = mKeyboard->isKeyDown(OIS::KC_A)? 1:0; // left
        movementCommands[2] = mKeyboard->isKeyDown(OIS::KC_S)? 1:0; // down
        movementCommands[3] = mKeyboard->isKeyDown(OIS::KC_D)? 1:0; // right
        
        //ROTATION
        rotationCommands[0] = mKeyboard->isKeyDown(OIS::KC_UP   )? 1:0; // up 
        rotationCommands[1] = mKeyboard->isKeyDown(OIS::KC_LEFT )? 1:0; // left
        rotationCommands[2] = mKeyboard->isKeyDown(OIS::KC_DOWN )? 1:0; // down
        rotationCommands[3] = mKeyboard->isKeyDown(OIS::KC_RIGHT)? 1:0; // right
        rotationCommands[4] = mKeyboard->isKeyDown(OIS::KC_SPACE)? 1:0; // reset
        
        mPhysics->move(movementCommands, rotationCommands, evt.timeSinceLastFrame);

        mCamera->setPosition(
            mPhysics->paddlePosition.x, 
            mPhysics->paddlePosition.y -250, 
            mPhysics->paddlePosition.z -200);

        mCamera->lookAt(mPhysics->paddlePosition);
    }
    return super;
}

bool Application::keyPressed( const OIS::KeyEvent &arg )
{
    bool super = BaseApplication::keyPressed( arg );
    
    if (arg.key == OIS::KC_M)
    {
        mSound->toggleSound();
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
