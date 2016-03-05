/*
-----------------------------------------------------------------------------
Filename:    Application.h
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

#ifndef __Application_h_
#define __Application_h_

#include "BaseApplication.h"
#include "Ball.h"
#include "PlayingField.h"
#include "Paddle.h"
#include "SoundPlayer.h"
#include "Scoreboard.h"

//---------------------------------------------------------------------------

class Application : public BaseApplication
{
public:
    Application(void);
    virtual ~Application(void);

protected:
  Ball* mBall;
  PlayingField* mField;
  Paddle* mPaddle;
  PhysicsWorld* mPhysics;
  SoundPlayer* mSound;
  Scoreboard* mScore;

  OgreBites::ParamsPanel* mScorePanel; 

  int movementCommands[4];
  int rotationCommands[5];
  virtual void createScene(void);
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  virtual bool keyPressed( const OIS::KeyEvent &arg );
};


//---------------------------------------------------------------------------

#endif // #ifndef __Application_h_

//---------------------------------------------------------------------------
