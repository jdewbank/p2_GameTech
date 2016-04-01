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
    if(multiplayerFlag) 
         std::cout << "**********************************Multiplayer!**********************************\n";
    else std::cout << "**********************************Singleplayer!*********************************\n";

    if(multiplayerFlag) setupNetwork();

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
    if(multiplayerFlag && !server) //player 2
    {
        mCamera->setFixedYawAxis(true,-Ogre::Vector3::UNIT_Y);
        mCamera->setPosition(0, 550, -200);
        mCamera->lookAt(0, 200, 0);
    }
    else //player 1
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
    if(multiplayerFlag)
    { 
        items.push_back("Player 1");
        items.push_back("Player 2");
    }
    else //singleplayer
    {
        items.push_back("Score");
        items.push_back("Highscore");   
    }

    if(mTrayMgr)
        mScorePanel = mTrayMgr->createParamsPanel(OgreBites::TL_BOTTOMRIGHT, "ScorePanel", 200, items);


}

bool Application::setupNetwork()
{


    net = new NetManager();
    bool result = net->initNetManager();

    std::cout << "Init Result: " << result << std::endl;

    if (server)
    {
        net->addNetworkInfo(PROTOCOL_UDP,NULL,63281);
        bool startResult = net->startServer();

        net->acceptConnections();
        while (!net->pollForActivity());
        net->denyConnections();

    }

    else //client
    {
        net->addNetworkInfo(PROTOCOL_UDP,host,63281);
        bool startResult = net->startClient();


        const char buf[128] = {};

        net->messageServer(PROTOCOL_UDP,buf,sizeof(buf));
    }

    return result;
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool super = BaseApplication::frameRenderingQueued(evt);

    if(super)
    {
        if(mPhysics == NULL)
            return super;

        // if(net)
        //     std::cout << "NETMANAGER" << std::endl;
        // else
        //     std::cout << "NO NETMANAGER" << std::endl;
        


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
        
        if(multiplayerFlag) 
        {
            Ogre::String score1 = Ogre::StringConverter::toString(mScore->getScore(1));
            Ogre::String score2 = Ogre::StringConverter::toString(mScore->getScore(2));
            mScorePanel->setParamValue(0, score1 );
            mScorePanel->setParamValue(1, score2 );
        }
        else //singleplayer
        {
            Ogre::String score = Ogre::StringConverter::toString(mScore->getScore());
            Ogre::String highscore = Ogre::StringConverter::toString(mScore->getHighScore());
            mScorePanel->setParamValue(0, score );
            mScorePanel->setParamValue(1, highscore );    
        }
        
        if(!multiplayerFlag)
        {
            mCamera->setPosition(
                mPhysics->paddle1Position.x, 
                mPhysics->paddle1Position.y -250, 
                mPhysics->paddle1Position.z -200);

            mCamera->lookAt(mPhysics->paddle1Position);
        }


        Ogre::Vector3 otherLocation;
        if(multiplayerFlag)
        {
            if(server)
            {
                const char buf[128] = {0};

                char* c_buf = (char*) buf;
                c_buf[0] = 's';

                float* f_buf = (float*)&buf[1];
                f_buf[0] = (float) mPhysics->paddle1Position.x;
                f_buf[1] = (float) mPhysics->paddle1Position.y;
                f_buf[2] = (float) mPhysics->paddle1Position.z;

                // f_buf[3] = (float) mPhysics->paddle1Quaternion.w;
                // f_buf[4] = (float) mPhysics->paddle1Quaternion.x;
                // f_buf[5] = (float) mPhysics->paddle1Quaternion.y;
                // f_buf[6] = (float) mPhysics->paddle1Quaternion.z;

                f_buf[3] = (float) mPhysics->paddle1Pitch;
                f_buf[4] = (float) mPhysics->paddle1Yaw;
                f_buf[5] = (float) mPhysics->paddle1Roll;


                net->messageClients(PROTOCOL_UDP, buf, sizeof(buf));
                std::cout << "Sending Paddle 1 position" << std::endl;

            }
            else
            {
                const char buf[128] = {0};

                char* c_buf = (char*) buf;
                c_buf[0] = 'c';

                float* f_buf = (float*)&buf[1];
                f_buf[0] = (float) mPhysics->paddle2Position.x;
                f_buf[1] = (float) mPhysics->paddle2Position.y;
                f_buf[2] = (float) mPhysics->paddle2Position.z;

                // f_buf[3] = (float) mPhysics->paddle2Quaternion.w;
                // f_buf[4] = (float) mPhysics->paddle2Quaternion.x;
                // f_buf[5] = (float) mPhysics->paddle2Quaternion.y;
                // f_buf[6] = (float) mPhysics->paddle2Quaternion.z;

                f_buf[3] = (float) mPhysics->paddle2Pitch;
                f_buf[4] = (float) mPhysics->paddle2Yaw;
                f_buf[5] = (float) mPhysics->paddle2Roll;


                net->messageServer(PROTOCOL_UDP,buf,sizeof(buf));
                std::cout << "Sending Paddle 2 position" << std::endl;
            }
        }



        if(multiplayerFlag) 
        {
            if(net->scanForActivity())
            {
                if(server)
                {
                    for(int i = 0; i < net->udpClientData.size(); ++i)
                    {
                        ClientData* cd = net->udpClientData[i];

                        if(cd->updated)
                        {                      
                            char* buf = cd->output;

                            if(buf[0] == 'c')
                            {
                                std::cout << "Receiving Paddle 2 position" << std::endl;
                                float* f_buf = (float*)&buf[1];

                                mPhysics->paddle2Position = Ogre::Vector3( 
                                    (float)f_buf[0], 
                                    (float)f_buf[1], 
                                    (float)f_buf[2]);

                                // mPhysics->paddle2Quaternion = Ogre::Quaternion(
                                //     (float)f_buf[3],
                                //     (float)f_buf[4],
                                //     (float)f_buf[5],
                                //     (float)f_buf[6]);

                                mPhysics->paddle2Pitch = (float)f_buf[3];
                                mPhysics->paddle2Yaw = (float)f_buf[4];
                                mPhysics->paddle2Roll = (float)f_buf[5];
                            }
                        }


                   }
                } 
                else //client
                {   

    

                    ClientData cd = net->udpServerData[0];

                    if(cd.updated)
                    {
                    
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
                        else if(buf[0] == 's')
                        {
                            std::cout << "Receiving Paddle 1 position" << std::endl;

                            float* f_buf = (float*)&buf[1];
                            mPhysics->paddle1Position = Ogre::Vector3( 
                                (float)f_buf[0], 
                                (float)f_buf[1], 
                                (float)f_buf[2]);

                            // mPhysics->paddle1Quaternion = Ogre::Quaternion(
                            //     (float)f_buf[3],
                            //     (float)f_buf[4],
                            //     (float)f_buf[5],
                            //     (float)f_buf[6]);

                            mPhysics->paddle1Pitch = (float)f_buf[3];
                            mPhysics->paddle1Yaw = (float)f_buf[4];
                            mPhysics->paddle1Roll = (float)f_buf[5];
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
       
        if(argc > 1) 
        {
            Ogre::String mode = argv[1];
            
            if (mode == "single") 
            {
                app.multiplayerFlag = false;
            } 
            else if (mode == "server") 
            {
                app.multiplayerFlag = true;
                app.server = true;
            } 
            else if (mode == "client") 
            {
                app.multiplayerFlag = true;
                app.server = false;
                
                if(argc > 2)
                    app.host = argv[2];
                else
                    std::cout << "You need to provide a host name if you are a client." << std::endl;
            }
        } 
        else 
        {
            app.multiplayerFlag = false;
        }
        
        try 
        {
            app.go();
        } 
        catch(Ogre::Exception& e)  
        {

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
