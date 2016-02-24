#include "Ball.h"

Ball::Ball(Ogre::SceneManager* scnMgr) { 
    srand(time(NULL));

    Ogre::Entity* ball = scnMgr->createEntity("sphere.mesh"); 
    // ball->setMaterialName("BallColor/CubeMap"); 
    ball->setCastShadows(true); 
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(); 
    rootNode->attachObject(ball); 
    rootNode->scale(0.1f,0.1f,0.1f); 
    bRadius = 10.0f; 
    
    bDirection = Ogre::Vector3(rand(), rand(), rand()); 
    bDirection.normalise(); 

    bSpeed = rand() % 400 + 100; 
} 

void Ball::move(const Ogre::FrameEvent& evt) { 
    Ogre::Vector3 bPosition = rootNode->getPosition(); 
    if (bPosition.x < -grounds->getWidth()/2.0f + bRadius && bDirection.x < 0.0f) 
        bDirection.x = -bDirection.x; 
    if (bPosition.x > grounds->getWidth()/2.0f - bRadius && bDirection.x > 0.0f) 
        bDirection.x = -bDirection.x; 
    if (bPosition.y < -grounds->getHeight()/2.0f + bRadius && bDirection.y < 0.0f) 
        bDirection.y = -bDirection.y; 
    if (bPosition.y > grounds->getHeight()/2.0f - bRadius && bDirection.y > 0.0f) 
        bDirection.y = -bDirection.y; 
    if (bPosition.z < -grounds->getLength()/2.0f + bRadius && bDirection.z < 0.0f) 
        bDirection.z = -bDirection.z; 
    if (bPosition.z > grounds->getLength()/2.0f - bRadius && bDirection.z > 0.0f) 
        bDirection.z = -bDirection.z; 

    rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);  
}
