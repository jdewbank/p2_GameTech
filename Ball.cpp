#include "Ball.h"

Ball::Ball(Ogre::SceneManager* scnMgr, PhysicsWorld* phys) { 
    srand(time(NULL));

    world = phys;

    Ogre::Entity* ball = scnMgr->createEntity("sphere.mesh"); 
    // ball->setMaterialName("BallColor/CubeMap"); 

    ball->setCastShadows(true); 
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("ball"); 
    rootNode->attachObject(ball); 
    rootNode->scale(0.1f,0.1f,0.1f); 
    bRadius = 10.0f; 

    
    
    rootNode->translate(0,0,0);
    bSpeed = 1.0f;

    
    btCollisionShape* ballShape = new btSphereShape(btScalar(bRadius));
    world->addCollisionShape(ballShape);

    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(.1f);
    btVector3 inertia(0,0,0);

    startTransform.setOrigin(btVector3(0,0,0));

    ballShape->calculateLocalInertia(mass, inertia);

    btDefaultMotionState* ballMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ballMotionState, ballShape, inertia);
    btRigidBody* ballRB = new btRigidBody(rbInfo);

    ballRB->setRestitution(1);
    ballRB->setUserPointer(rootNode);

    world->addRigidBodyToDynamicsWorld(ballRB);
    
    //trackrigidbodywithname?

} 

