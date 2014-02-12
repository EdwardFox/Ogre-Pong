#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreWindowEventUtilities.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include "Ball.h"
#include "Bat.h"

class Base : public Ogre::WindowEventListener, public Ogre::FrameListener, OgreBites::SdkTrayListener
{
	public:
		Base(void);
		virtual ~Base(void);
		bool run(void);

	protected:
		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		virtual void setupResources();
		virtual void createCamera();
		virtual void createScene();
		virtual void createOverlay();
		virtual void setupInput();
		virtual void updateInput();
		virtual void updateAI();

	private:
		void createFloor();

		Ogre::Root* mRoot;

		Ogre::String mPluginsCfg;
		Ogre::String mResourcesCfg;

		Ogre::RenderWindow* mWindow;
		Ogre::SceneManager* mSceneMgr;
		Ogre::Camera* mCamera;

		OIS::InputManager* mInputManager;
		OIS::Mouse* mMouse;
		OIS::Keyboard* mKeyboard;

		Ogre::OverlaySystem *mOverlaySystem;
		OgreBites::InputContext mInputContext;
		OgreBites::SdkTrayManager*	mTrayMgr;
		OgreBites::SdkCameraMan* mCameraMan;     	// basic camera controller
		OgreBites::ParamsPanel* mDetailsPanel;

		Ball mBall;
		Bat mPlayer;
		Bat mAI;

		float mSpeed;
};

