#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* scnMgr, Ogre::Vector3 paddleSpecs, Ogre::Real fieldSize, PhysicsWorld* phys) { 

    world = phys;

    Ogre::Entity* paddle = scnMgr->createEntity("cube.mesh"); 
    paddle->setMaterialName("PaddleMaterial"); 

    paddle->setCastShadows(true); 
    paddleNode = scnMgr->getRootSceneNode()->createChildSceneNode("paddle"); 
    std::cout << "Created sceneNode with name: " << paddleNode->getName() << std::endl;
    paddleNode->attachObject(paddle); 
    paddleNode->scale(paddleSpecs);

    paddleNode->translate(0,0,fieldSize/2 - 30);
    
    btScalar paddleX = paddleSpecs.x * 50;
    btScalar paddleY = paddleSpecs.y * 50;
    btScalar paddleZ = paddleSpecs.z * 50;
    btVector3 whl = btVector3(paddleX, paddleY, paddleZ);
    btCollisionShape* paddleShape = new btBoxShape(whl);
    world->addCollisionShape(paddleShape);

    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(.0f);
    btVector3 inertia(0,0,0);

    startTransform.setOrigin(btVector3(0,0,fieldSize/2 - 30));

    //left/right, forward/back, spin
    startTransform.setRotation(btQuaternion(0,0,0));

    paddleShape->calculateLocalInertia(mass, inertia);

    btDefaultMotionState* paddleMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, paddleMotionState, paddleShape, inertia);
    btRigidBody* paddleRB = new btRigidBody(rbInfo);

    paddleRB->setRestitution(1.5);
    paddleRB->setUserPointer(paddleNode);
    paddleRB->setLinearFactor(btVector3(1,1,0));
    paddleRB->setAngularFactor(btVector3(0,0,0));
    //body->setCollisionFlags( cmesh->actor->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    paddleRB->setActivationState( DISABLE_DEACTIVATION );

    world->addRigidBodyToDynamicsWorld(paddleRB);
    
    //trackrigidbodywithname?

} 

