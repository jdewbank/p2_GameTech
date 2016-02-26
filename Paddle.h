#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

#include "PhysicsWorld.h"

class Paddle { 
	protected: 
		Ogre::SceneNode* paddleNode; 
		Ogre::Real Xcoordinate;
		Ogre::Real Ycoordinate; 
		PhysicsWorld* world;

	public: 
		Paddle(Ogre::SceneManager* scnMgr, Ogre::Vector3 paddleSpecs, Ogre::Real fieldSize, PhysicsWorld* phys); 
		~Paddle(); 
		Ogre::SceneNode* getNode() { return paddleNode; } 
};
