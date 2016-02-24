#ifndef __PhysicsWorld_h_
#define __PhysicsWorld_h_

#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <map>
#include <string>


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

public:
    PhysicsWorld(void);
    void addRigidBodyToDynamicsWorld(btRigidBody*);
    void addCollisionShape(btCollisionShape*);
    void stepSimulation(btScalar);
    void move(void);
};

#endif // #ifndef __PhysicsWorld_h_