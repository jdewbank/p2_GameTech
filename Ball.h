#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

#include "PhysicsWorld.h"

class Ball { 
	protected: 
		Ogre::SceneNode* rootNode; 
		Ogre::Real bRadius; 
		Ogre::Vector3 bDirection; 
		Ogre::Real bSpeed; 
		PhysicsWorld* world;

	public: 
		Ball(Ogre::SceneManager* scnMgr, PhysicsWorld* phys); 
		~Ball(); 
		Ogre::SceneNode* getNode() { return rootNode; } 
};
