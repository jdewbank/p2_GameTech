#include <Ogre.h>

#include <OgreEntity.h> 
#include <OgreSceneManager.h> 

class Ball { 
	protected: 
		Ogre::SceneNode* rootNode; 
		Ogre::Real bRadius; 
		Ogre::Vector3 bDirection; 
		Ogre::Real bSpeed; 
		PlayingField* grounds; 

	public: 
		Ball(Ogre::SceneManager* scnMgr); 
		~Ball(); 
		void move(const Ogre::FrameEvent& evt); 
		Ogre::SceneNode* getNode() { return rootNode; } 
		void setPlayingField(PlayingField * pf) { grounds = pf; } 
};
