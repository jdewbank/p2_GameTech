#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

#include "PhysicsWorld.h"

class PlayingField {	
	protected: 
		Ogre::SceneNode* rootNode; 
		Ogre::Vector3 cSizeWHL; 
        PhysicsWorld* world;

	public: 
		PlayingField(Ogre::SceneManager*, Ogre::Vector3, PhysicsWorld*); 
		~PlayingField(); 

		Ogre::Real getWidth();
		Ogre::Real getHeight();
		Ogre::Real getLength();

};