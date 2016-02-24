#include "PlayingField.h"

PlayingField::PlayingField(Ogre::SceneManager* scnMgr, Ogre::Vector3 whl) {
    cSizeWHL = Ogre::Vector3(whl.x, whl.y, whl.z);

    //Positive X plane
    Ogre::Plane posXplane(Ogre::Vector3::UNIT_X, -cSizeWHL.x/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "posX",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        posXplane, 
        cSizeWHL.y, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* posX = scnMgr->createEntity("posX");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(posX);

    posX->setCastShadows(false);
    posX->setMaterialName("Examples/Rockwall");

    //Negative X plane
    Ogre::Plane negXplane(-1*Ogre::Vector3::UNIT_X, -cSizeWHL.x/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "negX",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        negXplane, 
        cSizeWHL.y, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* negX = scnMgr->createEntity("negX");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(negX);

    negX->setCastShadows(false);
    negX->setMaterialName("Examples/Rockwall");

    //Negative Y plane
    Ogre::Plane negYplane(-1*Ogre::Vector3::UNIT_Y, -cSizeWHL.y/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "negY",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        negYplane, 
        cSizeWHL.x, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* negY = scnMgr->createEntity("negY");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(negY);

    negY->setCastShadows(false);
    negY->setMaterialName("Examples/Rockwall");
    
    //Positive Y plane
    Ogre::Plane posYplane(Ogre::Vector3::UNIT_Y, -cSizeWHL.y/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "posY",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        posYplane, 
        cSizeWHL.x, cSizeWHL.z, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* posY = scnMgr->createEntity("posY");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(posY);

    posY->setCastShadows(false);
    posY->setMaterialName("Examples/Rockwall");

    //Negative Z plane
    Ogre::Plane negZplane(-1*Ogre::Vector3::UNIT_Z, -cSizeWHL.y/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "negZ",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        negZplane, 
        cSizeWHL.x, cSizeWHL.y, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_X);

    Ogre::Entity* negZ = scnMgr->createEntity("negZ");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(negZ);

    negZ->setCastShadows(false);
    negZ->setMaterialName("Examples/Rockwall");
    
    //Positive Z plane
    Ogre::Plane posZplane(Ogre::Vector3::UNIT_Z, -cSizeWHL.y/2.0f);
    Ogre::MeshManager::getSingleton().createPlane(
        "posZ",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        posZplane, 
        cSizeWHL.x, cSizeWHL.y, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_X);

    Ogre::Entity* posZ = scnMgr->createEntity("posZ");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(posZ);

    posZ->setCastShadows(false);
    posZ->setMaterialName("Examples/Rockwall");
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