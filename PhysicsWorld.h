#ifndef __PhysicsWorld_h_
#define __PhysicsWorld_h_

#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <map>
#include <string>

#include "SoundPlayer.h"
#include "Scoreboard.h"

class PhysicsWorld
{
protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;

    SoundPlayer* physSound;
    Scoreboard * physScore;
    bool multiplayer;
    bool server;

    int collisionIgnoreTimer;
    bool collisionIgnore;
    int lastPaddleHit;

public:

    Ogre::Vector3    paddle1Position;
    Ogre::Quaternion paddle1Quaternion;
    float paddle1Pitch;
    float paddle1Yaw;
    float paddle1Roll;

    Ogre::Vector3    paddle2Position;
    Ogre::Quaternion paddle2Quaternion;
    float paddle2Pitch;
    float paddle2Yaw;
    float paddle2Roll;

    PhysicsWorld(void);
    void addRigidBodyToDynamicsWorld(btRigidBody*);
    void addCollisionShape(btCollisionShape*);
    void detectCollisions(void);
    void move(int[], int[], btScalar);
    
    void setSoundPlayer(SoundPlayer* mSound) 
    { physSound = mSound; };
    void setScoreboard(Scoreboard* mScore) 
    { physScore = mScore; };
    void setMultiplayer(bool option = true)
    { multiplayer = option; };
    void setServer(bool option = true)
    { server = option; };
};

#endif // #ifndef __PhysicsWorld_h_