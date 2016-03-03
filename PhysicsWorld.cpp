#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(void) 
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

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
        btCollisionObject* objectA = const_cast<btCollisionObject*>
          ((btCollisionObject*)contactManifold->getBody0());
        btCollisionObject* objectB = const_cast<btCollisionObject*>
          ((btCollisionObject*)contactManifold->getBody1());

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

                    if(userPointerA)
                    {
                        Ogre::SceneNode* snA = static_cast<Ogre::SceneNode*>(userPointerA);

                        if(snA->getName() == "ball")
                        {
                            //std::cout << "ball collides A!" << std::endl;
                        }
                    }

                    if(userPointerB)
                    {
                        Ogre::SceneNode* snB = static_cast<Ogre::SceneNode*>(userPointerB);

                        if(snB->getName() == "ball")
                        {
                            //std::cout << "ball collides B!" << std::endl;
                        }
                    }
                }


            }
        }

    }
}

void PhysicsWorld::move(int paddleTranslate[], int paddleRotate[], btScalar frameTime)
{

    dynamicsWorld->stepSimulation(frameTime);

    detectCollisions();

    //std::cout << collisionShapes.size() << std::endl;
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

                    // body->applyForce(btVector3(0,0,forceDir * 10), btVector3(0,0,0));
                    //std::cout << "BALLS!" << std::endl;

                } 
                else if(sn->getName() == "paddle")
                {

                    btTransform newTrans = transform;
                    float paddleMoveSpeed = 0.06f;

                    //TRANSLATION
                    // up
                    if(paddleTranslate[0]) {
                        if(newTrans.getOrigin().getY() < (100.0f - 30.0f/2.0f)) {
                            newTrans.getOrigin() += (btVector3(0, paddleMoveSpeed, 0));
                        }

                    }

                    // left
                    if(paddleTranslate[1]) {
                        if(newTrans.getOrigin().getX() > (-100.0f + 40.0f/2.0f)) {
                            newTrans.getOrigin() += (btVector3(-paddleMoveSpeed, 0, 0));
                        }
                    }

                    // down
                    if(paddleTranslate[2]) {
                        if(newTrans.getOrigin().getY() > (-100.0f + 30.0f/2.0f)) {
                            newTrans.getOrigin() += (btVector3(0, -paddleMoveSpeed, 0));
                        }
                    }

                    //right
                    if(paddleTranslate[3]) {
                        if(newTrans.getOrigin().getX() < (100.0f - 40.0f/2.0f)) {
                            newTrans.getOrigin() += (btVector3(paddleMoveSpeed, 0, 0));
                        }
                    }

                    btScalar rotationSpeed = 1.0f;

                    // std::cout << "ROTATE!" << std::endl;
                    
                    // ROTATION
                    // up
                    if(paddleRotate[0]) {
                        btQuaternion rotation = newTrans.getRotation();
                        rotation += btQuaternion(0,rotationSpeed,0);
                        newTrans.setRotation(rotation);
                    }
                    // left
                    if(paddleRotate[1]) {
                        btQuaternion rotation = newTrans.getRotation();
                        rotation += btQuaternion(rotationSpeed,0,0);
                        newTrans.setRotation(rotation);
                    }

                    // down
                    if(paddleRotate[2]) {
                        btQuaternion rotation = newTrans.getRotation();
                        rotation += btQuaternion(0,-rotationSpeed,0);
                        newTrans.setRotation(rotation);
                    }

                    //right
                    if(paddleRotate[3]){
                        btQuaternion rotation = newTrans.getRotation();
                        rotation += btQuaternion(-rotationSpeed,0,0);
                        newTrans.setRotation(rotation);
                    }

                    body->getMotionState()->setWorldTransform(newTrans);
                    //body->getMotionState()->getWorldTransform(transform);
                    //btVector3 tempOrigin = transform.getOrigin();
                    body->translate(newTrans.getOrigin() - body->getCenterOfMassPosition());
                    //std::cout << "Moving paddle transform to (" << tempOrigin.getX() << ", " << tempOrigin.getY() << ", " << tempOrigin.getZ() << ")\n";
                    //std::cout << "Paddle rigid body is now at (" << body->getCenterOfMassPosition().getX() << ", " << body->getCenterOfMassPosition().getY() << ", " << body->getCenterOfMassPosition().getZ() << ")\n";
                }
                    
                sn->setPosition(Ogre::Vector3(origin.getX(), origin.getY(), origin.getZ()));
                sn->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
            }
        }
    }
}