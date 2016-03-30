#include "PlayingField.h"

PlayingField::PlayingField(Ogre::SceneManager* scnMgr, Ogre::Vector3 whl, PhysicsWorld* phys, bool multiplayerFlag) {
    cSizeWHL = Ogre::Vector3(whl.x, whl.y, whl.z);

    multiplayerFlag = multiplayerFlag;
    float planeScale = 1.0f;
    if(multiplayerFlag) planeScale = 2.0f;

    world = phys;

    btScalar planeMass(0.);
    btVector3 localPlaneInertia(0,0,0);

    btScalar wallRestitution(0.7f);
    btScalar groundRestitution(1.0f);

    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("room");

    std::string materialName = "Examples/Rockwall";

    //Negative X plane
    Ogre::Plane negXplane(Ogre::Vector3::UNIT_X, -cSizeWHL.x/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "negX",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        negXplane, 
        cSizeWHL.y*planeScale, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* negX = scnMgr->createEntity("negX");
    Ogre::SceneNode* negXnode = rootNode->createChildSceneNode();
    negXnode->attachObject(negX);

    if(multiplayerFlag) negXnode->setPosition(Ogre::Vector3(0.0f, cSizeWHL.x/2.0f, 0.0f));

    negX->setCastShadows(false);
    negX->setMaterialName(materialName);

    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(-cSizeWHL.x/2.0f, 0, 0));

        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(1,0,0), 0);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

        groundShape->calculateLocalInertia(planeMass, localPlaneInertia);

        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(planeMass, groundMotionState, groundShape, localPlaneInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);

        groundBody->setRestitution(wallRestitution);

        world->addRigidBodyToDynamicsWorld(groundBody);
    }



    //Positive X plane
    Ogre::Plane posXplane(-1*Ogre::Vector3::UNIT_X, -cSizeWHL.x/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "posX",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        posXplane, 
        cSizeWHL.y*planeScale, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* posX = scnMgr->createEntity("posX");
    Ogre::SceneNode* posXnode = rootNode->createChildSceneNode();
    posXnode->attachObject(posX);

    if(multiplayerFlag) posXnode->setPosition(Ogre::Vector3(0.0f, cSizeWHL.x/2.0f, 0.0f));

    posX->setCastShadows(false);
    posX->setMaterialName(materialName);

    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(cSizeWHL.x/2.0f, 0, 0));

        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(-1,0,0), 0);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

        groundShape->calculateLocalInertia(planeMass, localPlaneInertia);

        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(planeMass, groundMotionState, groundShape, localPlaneInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);

        groundBody->setRestitution(wallRestitution);

        world->addRigidBodyToDynamicsWorld(groundBody);
    }

    //Positive Y plane
    float posYdisplacement = 0.5f;
    if(multiplayerFlag) {
        posYdisplacement = 1.5f;
    } 
    Ogre::Plane posYplane(-1*Ogre::Vector3::UNIT_Y, -cSizeWHL.y*posYdisplacement);
    Ogre::MeshManager::getSingleton().createPlane(
        "posY",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        posYplane, 
        cSizeWHL.x, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* posY = scnMgr->createEntity("posY");
    rootNode->createChildSceneNode()->attachObject(posY);

    posY->setCastShadows(false);
    posY->setMaterialName(materialName);


    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, cSizeWHL.y, 0));

        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,-1,0), 0);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

        groundShape->calculateLocalInertia(planeMass, localPlaneInertia);

        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(planeMass, groundMotionState, groundShape, localPlaneInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);

        groundBody->setRestitution(wallRestitution);

        world->addRigidBodyToDynamicsWorld(groundBody);
    }

    //Negative Y plane
    Ogre::Plane negYplane(Ogre::Vector3::UNIT_Y, -cSizeWHL.y/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "negY",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        negYplane, 
        cSizeWHL.x, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* negY = scnMgr->createEntity("negY");
    rootNode->createChildSceneNode()->attachObject(negY);

    negY->setCastShadows(false);
    negY->setMaterialName(materialName);
    
    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -cSizeWHL.y/2.0f, 0));

        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0), 0);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

        groundShape->calculateLocalInertia(planeMass, localPlaneInertia);

        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(planeMass, groundMotionState, groundShape, localPlaneInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);

        groundBody->setRestitution(wallRestitution);

        world->addRigidBodyToDynamicsWorld(groundBody);
    }


    //Negative Z plane
    Ogre::Plane negZplane(Ogre::Vector3::UNIT_Z, -cSizeWHL.z/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "negZ",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        negZplane, 
        cSizeWHL.x, cSizeWHL.y, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_X);

    Ogre::Entity* negZ = scnMgr->createEntity("negZ");
    rootNode->createChildSceneNode()->attachObject(negZ);

    negZ->setCastShadows(false);
    negZ->setMaterialName(materialName);

    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, 0, -cSizeWHL.z/2.0f));

        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,0,1), 0);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

        groundShape->calculateLocalInertia(planeMass, localPlaneInertia);

        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(planeMass, groundMotionState, groundShape, localPlaneInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);

        groundBody->setRestitution(wallRestitution);

        world->addRigidBodyToDynamicsWorld(groundBody);
    }
    
    //Positive Z plane
    Ogre::Plane posZplane(-1*Ogre::Vector3::UNIT_Z, -cSizeWHL.z/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "posZ",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        posZplane, 
        cSizeWHL.x*planeScale, cSizeWHL.y, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_X);

    Ogre::Entity* posZ = scnMgr->createEntity("posZ");
    Ogre::SceneNode* floorNode = rootNode->createChildSceneNode("floor");
    floorNode->attachObject(posZ);

    if(multiplayerFlag) floorNode->setPosition(Ogre::Vector3(0.0f, cSizeWHL.z/2.0f, 0.0f));

    posZ->setCastShadows(false);
    posZ->setMaterialName(materialName);

    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, 0, cSizeWHL.z/2.0f));

        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,0,-1), 0);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

        groundShape->calculateLocalInertia(planeMass, localPlaneInertia);

        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(planeMass, groundMotionState, groundShape, localPlaneInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);

        groundBody->setRestitution(groundRestitution);
        groundBody->setUserPointer(floorNode);

        world->addRigidBodyToDynamicsWorld(groundBody);
    }
}

Ogre::Real PlayingField::getWidth() {
    return cSizeWHL[0];
}

Ogre::Real PlayingField::getHeight() {
    return cSizeWHL[1];
}

Ogre::Real PlayingField::getLength() {
    return cSizeWHL[2];
}