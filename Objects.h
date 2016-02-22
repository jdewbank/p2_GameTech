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
