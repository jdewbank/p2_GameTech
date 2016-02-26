#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* scnMgr, Ogre::Vector3 paddleSpecs, Ogre::Real fieldSize, PhysicsWorld* phys) { 
    srand(time(NULL));

    world = phys;

    Ogre::Entity* paddle = scnMgr->createEntity("cube.mesh"); 
    // paddle->setMaterialName("PaddleColor/CubeMap"); 

    paddle->setCastShadows(true); 
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("paddle"); 
    rootNode->attachObject(paddle); 
    rootNode->scale(paddleSpecs);

    rootNode->translate(0,0,fieldSize/2);
    
    btScalar paddleX = paddleSpecs.x;
    btScalar paddleY = paddleSpecs.y;
    btScalar paddleZ = paddleSpecs.z;
    btVector3 whl = btVector3(paddleX, paddleY, paddleZ);
    btCollisionShape* paddleShape = new btBoxShape(whl);
    world->addCollisionShape(paddleShape);

    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(.1f);
    btVector3 inertia(0,0,0);

    startTransform.setOrigin(btVector3(0,0,0));

    paddleShape->calculateLocalInertia(mass, inertia);

    btDefaultMotionState* paddleMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, paddleMotionState, paddleShape, inertia);
    btRigidBody* paddleRB = new btRigidBody(rbInfo);

    paddleRB->setRestitution(1);
    paddleRB->setUserPointer(rootNode);

    world->addRigidBodyToDynamicsWorld(paddleRB);
    
    //trackrigidbodywithname?

} 

