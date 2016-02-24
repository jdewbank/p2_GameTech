#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(void) 
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}


void PhysicsWorld::addRigidBodyToDynamicsWorld(btRigidBody* rb)
{
    dynamicsWorld->addRigidBody(rb);
}

void PhysicsWorld::addCollisionShape(btCollisionShape* cs)
{
    collisionShapes.push_back(cs);
}

void PhysicsWorld::stepSimulation(btScalar ts)
{
    dynamicsWorld->stepSimulation(ts);
}

void PhysicsWorld::move(void)
{
    for(int i = 0; i < collisionShapes.size(); ++i)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];

        btRigidBody* body = btRigidBody::upcast(obj);

        if(body && body->getMotionState())
        {
            btTransform transform;
            body->getMotionState()->getWorldTransform(transform);

            void* userPointer = body->getUserPointer();
            if(userPointer)
            {
                btQuaternion orientation = transform.getRotation();
                btVector3 origin = transform.getOrigin();

                Ogre::SceneNode* sn = static_cast<Ogre::SceneNode*>(userPointer);

                sn->setPosition(Ogre::Vector3(origin.getX(), origin.getY(), origin.getZ()));
                sn->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
            }
        }
    }
}