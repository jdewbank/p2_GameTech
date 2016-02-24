#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

class PlayingField {	
	protected: 
		Ogre::SceneNode* rootNode; 
		Ogre::Vector3 cSizeWHL; 

	public: 
		PlayingField(Ogre::SceneManager* scnMgr, Ogre::Vector3); 
		~PlayingField(); 

		Ogre::Real getWidth();
		Ogre::Real getHeight();
		Ogre::Real getLength();

};