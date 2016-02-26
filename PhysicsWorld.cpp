#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(void) 
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,0,0));
    forceDir = 1.0f;
    collisionIgnoreTimer = 20;
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

void PhysicsWorld::detectCollisions(void) 
{
    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();


    if(collisionIgnoreTimer > 0 && collisionIgnoreTimer != 20) 
    {
        collisionIgnoreTimer--;
    } else {
        collisionIgnoreTimer = 20;
    }

    for(int i = 0; i < numManifolds; ++i) 
    {
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* objectA = const_cast<btCollisionObject*>(contactManifold->getBody0());
        btCollisionObject* objectB = const_cast<btCollisionObject*>(contactManifold->getBody1());

        void* userPointerA = objectA->getUserPointer();
        void* userPointerB = objectB->getUserPointer();

        int numContacts = contactManifold->getNumContacts();


        for(int j = 0; j < numContacts; ++j) 
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if(pt.getDistance() <= 1.0f) 
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;

                //std::cout << collisionIgnoreTimer << std::endl;
                //std::cout << "collision detected!" << std::endl;    

                if(collisionIgnoreTimer == 20) 
                {
                    collisionIgnoreTimer--;
                    //collisionIgnore = true;

                    forceDir*=-1;

                    if(userPointerA)
                    {
                        Ogre::SceneNode* snA = static_cast<Ogre::SceneNode*>(userPointerA);

                        if(snA->getName() == "ball")
                        {
                            std::cout << "ball collides A!" << std::endl;
                        }
                    }

                    if(userPointerB)
                    {
                        Ogre::SceneNode* snB = static_cast<Ogre::SceneNode*>(userPointerB);

                        if(snB->getName() == "ball")
                        {
                            std::cout << "ball collides B!" << std::endl;
                        }
                    }
                }


            }
        }

    }
}

void PhysicsWorld::move(void)
{

    detectCollisions();
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

                if(sn->getName() == "ball")
                {
                    body->applyForce(btVector3(forceDir * 25,0,0), btVector3(0,0,0));
                    std::cout << "BALLS!" << std::endl;

                }
                    
                sn->setPosition(Ogre::Vector3(origin.getX(), origin.getY(), origin.getZ()));
                sn->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
            }
        }
    }
}