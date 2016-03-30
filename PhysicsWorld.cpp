#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(void) 
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,0,60));

    collisionIgnoreTimer = 20;
    paddlePosition = Ogre::Vector3(0,0,0);
    lastPaddleHit = 1;

    collisionIgnore = false;
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

    int ballContacts = 0;


    for(int i = 0; i < numManifolds; ++i) 
    {
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* objectA = const_cast<btCollisionObject*>
          ((btCollisionObject*)contactManifold->getBody0());
        btCollisionObject* objectB = const_cast<btCollisionObject*>
          ((btCollisionObject*)contactManifold->getBody1());

        void* userPointerA = objectA->getUserPointer();
        void* userPointerB = objectB->getUserPointer();

        Ogre::SceneNode* snA;
        Ogre::SceneNode* snB;

        if(userPointerA) {
            snA = static_cast<Ogre::SceneNode*>(userPointerA);

        }

        if(userPointerB) {
            snB = static_cast<Ogre::SceneNode*>(userPointerB);
        }

        int numContacts = contactManifold->getNumContacts();

        if(snA) {
            if(snA->getName() == "ball") {
                ballContacts += numContacts;
            }
        }

        for(int j = 0; j < numContacts; ++j) 
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);

            //std::cout << pt.getDistance() << "\n";
            if(pt.getDistance() <= 1.0f) 
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;

                //std::cout << collisionIgnoreTimer << std::endl;
                //std::cout << "collision detected!" << std::endl;    

                if(!collisionIgnore) 
                {

                    if(userPointerA)
                    {
                        if(snA->getName() == "ball") {
                            collisionIgnore = true;
                        }
                        //std::cout << "A is " << snA->getName() << std::endl;
                        if(userPointerB)
                        {
                            //std::cout << "B is " << snB->getName() << std::endl;
                            
                            if (snA->getName() == "floor" ||
                                snB->getName() == "floor" )
                            {
                                // Floor collision
                                physSound->playSound(2);

                                if(snA->getName() == "ball") {
                                    if(snA->getPosition().y > 100.0f) {
                                        std::cout << "Player 1 gets a point!\n";
                                        physScore->addScore(1,1);
                                    } else {
                                        std::cout << "Player 2 gets a point!\n";
                                        physScore->addScore(1,2);
                                    }
                                } else {
                                    if(snA->getPosition().y > 100.0f) {
                                        std::cout << "Player 1 gets a point!\n";
                                        physScore->addScore(1,1);
                                    } else {
                                        std::cout << "Player 2 gets a point!\n";
                                        physScore->addScore(1,2);
                                    }
                                }
                                //physScore->resetScore();
                            }
                            else if (
                                snA->getName() == "paddle" ||
                                snB->getName() == "paddle" )
                            {
                                // Paddle collision
                                physSound->playSound(1);
                                //physScore->addScore(1);  

                                lastPaddleHit = 1;  
                            }      
                            else if (
                                snA->getName() == "paddle2" ||
                                snB->getName() == "paddle2" )
                            {
                                // Paddle collision
                                physSound->playSound(1);
                                //physScore->addScore(1);    

                                lastPaddleHit = 2;
                            } 
                        }
                        else 
                        {
                            // Wall collision
                            physSound->playSound(0);
                            //physScore->addScore(3);
                        }
                    }
                    //std::cout << "Player 1: " << physScore->getScore(1) << std::endl;
                    //std::cout << "Player 2: " << physScore->getScore(2)  << std::endl;
                }
            }
        }
    }

    if(ballContacts == 0) {
        collisionIgnore = false;
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

                Ogre::SceneNode* sn = static_cast<Ogre::SceneNode*>(userPointer);

                btVector3    origin = transform.getOrigin();
                btQuaternion orientation = transform.getRotation();

                if(sn->getName() == "ball")
                {
                    //Ball's behavior
                } 

                else if(sn->getName() == "paddle" || sn->getName() == "paddle2")
                {
                    int paddleMult = 0;
                    if(sn->getName() == "paddle2")
                        paddleMult = 1;

                    float paddleMoveSpeed = 0.06f;
                    btScalar rotationSpeed = .005f;

                    btTransform newTrans = transform;

                    //TRANSLATION
                    // up
                    if(paddleTranslate[0]) {
                        if(newTrans.getOrigin().getY() < (100.0f - 30.0f/2.0f + paddleMult*200.0f)) {
                            newTrans.getOrigin() += (btVector3(0, paddleMoveSpeed * (1000.0f * frameTime), 0));
                        }
                    }

                    // left
                    if(paddleTranslate[1]) {
                        if(newTrans.getOrigin().getX() < (100.0f - 40.0f/2.0f)) {
                            newTrans.getOrigin() += (btVector3(paddleMoveSpeed * (1000.0f * frameTime), 0, 0));
                        }
                    }

                    // down
                    if(paddleTranslate[2]) {
                        if(newTrans.getOrigin().getY() > (-100.0f + 30.0f/2.0f + paddleMult*200.0f)) {
                            newTrans.getOrigin() += (btVector3(0, -paddleMoveSpeed * (1000.0f * frameTime), 0));
                        }
                    }

                    //right
                    if(paddleTranslate[3]) {
                        if(newTrans.getOrigin().getX() > (-100.0f + 40.0f/2.0f)) {
                            newTrans.getOrigin() += (btVector3(-paddleMoveSpeed * (1000.0f * frameTime), 0, 0));
                        }
                    }

                    btScalar roll, pitch, yaw;
                    btQuaternion rotation = newTrans.getRotation();
                    btMatrix3x3(rotation).getEulerYPR(yaw,pitch,roll);

                    // ROTATION
                    // up
                    if(paddleRotate[0] && roll < 1) {
                        roll += rotationSpeed * (1000.0f * frameTime);
                    }
                    // left
                    if(paddleRotate[1] && pitch > -1) {
                        pitch -= rotationSpeed * (1000.0f * frameTime);
                    }

                    // down
                    if(paddleRotate[2] && roll > -1) {
                        roll -= rotationSpeed * (1000.0f * frameTime);
                    }

                    //right
                    if(paddleRotate[3] && pitch < 1){
                        pitch += rotationSpeed * (1000.0f * frameTime);
                    }

                    //reset
                    if(paddleRotate[4]){
                        pitch = 0;
                        roll = 0;
                        yaw = 0;
                    }
                    
                    btQuaternion tmp;
                    tmp.setEulerZYX(yaw,pitch,roll);
                    newTrans.setRotation(tmp);

                    body->getMotionState()->setWorldTransform(newTrans);
                    body->translate(newTrans.getOrigin() - body->getCenterOfMassPosition());
                    body->setCenterOfMassTransform(newTrans);
                    
                    //std::cout << "Moving paddle transform to (" << tempOrigin.getX() << ", " << tempOrigin.getY() << ", " << tempOrigin.getZ() << ")\n";
                    //std::cout << "Paddle rigid body is now at (" << body->getCenterOfMassPosition().getX() << ", " << body->getCenterOfMassPosition().getY() << ", " << body->getCenterOfMassPosition().getZ() << ")\n";
                    
                }
                    
                paddlePosition = Ogre::Vector3(
                    origin.getX(), origin.getY(), origin.getZ());
                sn->setPosition(paddlePosition);
                sn->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
            }
        }
    }
}