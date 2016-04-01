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
    if(multiplayerFlag) std::cout << "**********************************Multiplayer!**********************************\n";
    else std::cout << "**********************************Singleplayer!**********************************\n";

    if(multiplayerFlag)
    {
        net = new NetManager();
        bool result = net->initNetManager();

        std::cout << "Init Result: " << result << std::endl;

        if (server)
        {
            net->addNetworkInfo(PROTOCOL_UDP,NULL,63281);
            bool startResult = net->startServer();

            std::cout << "Server Start Result: " << startResult << std::endl;

            net->acceptConnections();
            while (!net->pollForActivity());
            net->denyConnections();

            std::cout << "list length: " << net->udpClientData.size() << std::endl;

            for(int i = 0; i < net->udpClientData.size(); ++i)
            {
                ClientData* cd = net->udpClientData[i];


                float* f_buf = (float*) cd->output;

                for(int i = 0; i < 3; ++i)
                    std::cout << i << ": " << f_buf[i] << std::endl;

                std::cout << std::endl;

            }

        } 
        else 
        {
            net->addNetworkInfo(PROTOCOL_UDP,host,63281);
            bool startResult = net->startClient();

            float f1 = 10.6f;
            float f2 = -19.2f;
            float f3 = 0.0f;

            const char buf[128] = {};
            float* f_buf = (float*)buf;

            f_buf[0] = f1;
            f_buf[1] = f2;
            f_buf[2] = f3;

            net->messageServer(PROTOCOL_UDP,buf,sizeof(buf));

            std::cout << "Client Start Result: " << startResult << std::endl;
        }
    }


    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    //Set up Physics
    mPhysics = new PhysicsWorld();
    mPhysics->setMultiplayer(multiplayerFlag);
    mPhysics->setServer(server);

    //Lights
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(0, -50, -200);

    //Shadows
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    //Camera
    if(multiplayerFlag && !server)
    {
        mCamera->setFixedYawAxis(true,-Ogre::Vector3::UNIT_Y);
        mCamera->setPosition(0, 550, -200);
        mCamera->lookAt(0, 200, 0);
    }
    else
    {
        mCamera->setPosition(0, -350, -200);
        mCamera->lookAt(0, 0, 0);
    }

    //Sphere
    mBall = new Ball(mSceneMgr, mPhysics);

    Ogre::Real fieldSize = 200;

    //Paddle
    Ogre::Vector3 paddleSpecs = Ogre::Vector3(.4f,.3f,.1f);
    mPaddle = new Paddle(mSceneMgr, paddleSpecs, fieldSize, mPhysics, 1);

    if(multiplayerFlag) mPaddle2 = new Paddle(mSceneMgr, paddleSpecs, fieldSize, mPhysics, 2);

    //PlayingField    
    mField = new PlayingField(mSceneMgr, 
        Ogre::Vector3(fieldSize,fieldSize,fieldSize), mPhysics, multiplayerFlag);

    //Sounds
    mSound = new SoundPlayer(); 
    mPhysics->setSoundPlayer(mSound);

    //Score
    mScore = new Scoreboard(2, net, server);
    mPhysics->setScoreboard(mScore);

    Ogre::StringVector items;
    items.push_back("Player 1:");
    if(multiplayerFlag) items.push_back("Player 2:");

    if(mTrayMgr)
        mScorePanel = mTrayMgr->createParamsPanel(OgreBites::TL_BOTTOMRIGHT, "ScorePanel", 200, items);


}


bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool super = BaseApplication::frameRenderingQueued(evt);

    if(super)
    {
        if(mPhysics == NULL)
            return super;


        bool client = multiplayerFlag && !server;

        //TRANSLATION
        movementCommands[0] = mKeyboard->isKeyDown(OIS::KC_W)? !client:client; // up 
        movementCommands[1] = mKeyboard->isKeyDown(OIS::KC_A)? !client:client; // left
        movementCommands[2] = mKeyboard->isKeyDown(OIS::KC_S)? !client:client; // down
        movementCommands[3] = mKeyboard->isKeyDown(OIS::KC_D)? !client:client; // right
        
        //ROTATION
        rotationCommands[0] = mKeyboard->isKeyDown(OIS::KC_UP   )? !client:client; // up 
        rotationCommands[1] = mKeyboard->isKeyDown(OIS::KC_LEFT )? !client:client; // left
        rotationCommands[2] = mKeyboard->isKeyDown(OIS::KC_DOWN )? !client:client; // down
        rotationCommands[3] = mKeyboard->isKeyDown(OIS::KC_RIGHT)? !client:client; // right
        rotationCommands[4] = mKeyboard->isKeyDown(OIS::KC_SPACE)? 1:0; // reset
        
        mPhysics->move(movementCommands, rotationCommands, evt.timeSinceLastFrame);
        Ogre::String score1 = Ogre::StringConverter::toString(mScore->getScore(1));
        mScorePanel->setParamValue(0, score1 );
        if(multiplayerFlag) {
            Ogre::String score2 = Ogre::StringConverter::toString(mScore->getScore(2));
            mScorePanel->setParamValue(1, score2 );
        } 
        
        if(!multiplayerFlag) {
            mCamera->setPosition(
                mPhysics->paddlePosition.x, 
                mPhysics->paddlePosition.y -250, 
                mPhysics->paddlePosition.z -200);

            mCamera->lookAt(mPhysics->paddlePosition);
        }

        if(multiplayerFlag) {
            if(net->scanForActivity())
            {
                if(server) {
                    for(int i = 0; i < net->udpClientData.size(); ++i)
                    {
                        ClientData* cd = net->udpClientData[i];

                        if(cd->updated)
                            std::cout << "UPDATED!" << std::endl;
                    }
                } else {
                    for(int i = 0; i < 10; ++i)
                    {
                        ClientData cd = net->udpServerData[i];

                        if(cd.updated)
                        {
                            std::cout << "UPDATED!" << std::endl;

                            char* buf = cd.output;

                            if(buf[0] == 'a')
                            {
                                int points = (int)buf[1];
                                int player = (int)buf[2];
                                std::cout << "Add " << buf[1] << " points to " << buf[2] << std::endl; 
                                mScore->addScore(points,player);
                            }
                            else if(buf[0] == 'r')
                            {
                                int player = (int)buf[1];
                                std::cout << "Reset the points for player " << buf[1] <<  std::endl; 
                                mScore->resetScore(player);
                            }
                        }

                    }
                }
            }
        }

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
    else if (arg.key == OIS::KC_0)
    {
        mScore->resetAll();
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
        //std::cout << "TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST\n";
        if(argc > 1) {
            Ogre::String mode = argv[1];
            if(mode == "single") {
                app.multiplayerFlag = false;
            } else if (mode == "server") {
                app.multiplayerFlag = true;
                app.server = true;
            } else if (mode == "client") {
                app.multiplayerFlag = true;
                app.server = false;
                if(argc >2)
                    app.host = argv[2];
                else
                    std::cout << "You need to provide a host name if you are a client." << std::endl;
            }
        } else {
            app.multiplayerFlag = false;
        }
        

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
