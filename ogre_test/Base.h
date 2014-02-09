#pragma once

#include <OgreRoot.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreWindowEventUtilities.h>

class Base : public Ogre::WindowEventListener, public Ogre::FrameListener
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
		virtual void createScene();
		virtual void setupInput();

	private:
		Ogre::Root* mRoot;

		Ogre::String mPluginsCfg;
		Ogre::String mResourcesCfg;

		Ogre::RenderWindow* mWindow;
		Ogre::SceneManager* mSceneMgr;
		Ogre::Camera* mCamera;

		OIS::InputManager* mInputManager;
		OIS::Mouse* mMouse;
		OIS::Keyboard* mKeyboard;
};

