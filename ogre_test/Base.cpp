#include "Base.h"
#include <OgreException.h>

#include <OgreConfigFile.h>

#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <OgreMeshManager.h>

Base::Base(void) :
	mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mTrayMgr(0),
	mCameraMan(0),
	mDetailsPanel(0),
	mOverlaySystem(0),
	mBall(),
	mPlayer(),
	mAI(),
	mSpeed(1000)
{

}

Base::~Base(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;
	if (mOverlaySystem) delete mOverlaySystem;

	delete mRoot;
}

// Adjust mouse clipping area
void Base::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void Base::setupResources()
{
	// set up resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

void Base::setupInput()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem(pl);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
}

void Base::createCamera()
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it a 80 in Z direction
	mCamera->setPosition(Ogre::Vector3(0, 500, -1000));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera); 

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void Base::createFloor()
{
	// Plane
	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;
 
	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 500.0f, 1000.0f, 10, 10, true, 1, 50.0f, 50.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* planeEnt = mSceneMgr->createEntity("plane", "floor");
	planeEnt->setMaterialName("Examples/GrassFloor");
	planeEnt->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEnt);
}

void Base::createScene()
{
	createFloor();

	mPlayer.create("Player", mSceneMgr);
	mPlayer.translate(0.0, 51.0, -450.0);

	mAI.create("AI", mSceneMgr);
	mAI.translate(0.0, 51.0, 450.0);

	mBall.create("Ball", mSceneMgr);
	mBall.translate(0.0, 50.0, 0.0);
	mBall.setSpeed(mSpeed, 0.0, -mSpeed);

	// Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* directionalLight = mSceneMgr->createLight("directionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(.5, .5, .5));
    directionalLight->setSpecularColour(Ogre::ColourValue(.75, .75, .75));
	directionalLight->setDirection(Ogre::Vector3( 0, -1, 0 ));
}

void Base::createOverlay()
{
	mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	//mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();
 
	// create a params panel for displaying sample details
	Ogre::StringVector items;
	items.push_back("Ball X");
	items.push_back("Ball Y");
	items.push_back("Ball Z");
	items.push_back("");
	items.push_back("Player X");
	items.push_back("Player Y");
	items.push_back("Player Z");
	items.push_back("");
	items.push_back("AI X");
	items.push_back("AI Y");
	items.push_back("AI Z");

	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
}

// Unattach OIS before window shutdown (very important under Linux)
void Base::windowClosed(Ogre::RenderWindow* rw)
{
	// Only close for window that created OIS (the main window in these demos)
	if(rw == mWindow)
	{
		if(mInputManager)
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

void Base::updateInput()
{
	mPlayer.setSpeed(0.0, 0.0, 0.0);

	if(mKeyboard->isKeyDown(OIS::KC_A))
		mPlayer.setSpeed(mSpeed, 0.0, 0.0);

	if(mKeyboard->isKeyDown(OIS::KC_D))
		mPlayer.setSpeed(-mSpeed, 0.0, 0.0);

}

void Base::updateAI()
{
	auto ballX = mBall.getPosition().x;
	auto aiX = mAI.getPosition().x;

	if(ballX < aiX)
		mAI.setSpeed(-mSpeed, 0.0, 0.0);
	else if(ballX == aiX)
		mAI.setSpeed(0.0, 0.0, 0.0);
	else
		mAI.setSpeed(mSpeed, 0.0, 0.0);
}

bool Base::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mDetailsPanel->setParamValue(0,  Ogre::StringConverter::toString(mBall.getPosition().x));
	mDetailsPanel->setParamValue(1,  Ogre::StringConverter::toString(mBall.getPosition().y));
	mDetailsPanel->setParamValue(2,  Ogre::StringConverter::toString(mBall.getPosition().z));
	mDetailsPanel->setParamValue(4,  Ogre::StringConverter::toString(mPlayer.getPosition().x));
	mDetailsPanel->setParamValue(5,  Ogre::StringConverter::toString(mPlayer.getPosition().y));
	mDetailsPanel->setParamValue(6,  Ogre::StringConverter::toString(mPlayer.getPosition().z));
	mDetailsPanel->setParamValue(8,  Ogre::StringConverter::toString(mAI.getPosition().x));
	mDetailsPanel->setParamValue(9,  Ogre::StringConverter::toString(mAI.getPosition().y));
	mDetailsPanel->setParamValue(10,  Ogre::StringConverter::toString(mAI.getPosition().z));

	if(mWindow->isClosed())
		return false;

	// Need to caputre/update each device
	mKeyboard->capture();
	mMouse->capture();

	mTrayMgr->frameRenderingQueued(evt);

	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			
		}
	}

	// Input
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	// Updates
	updateInput();
	updateAI();

	mPlayer.update(evt.timeSinceLastEvent);
	mAI.update(evt.timeSinceLastEvent);
	mBall.update(evt.timeSinceLastEvent, mPlayer, mAI);

	if(mBall.getPosition().z < -500 || mBall.getPosition().z > 500)
		mBall.setPosition(0.0, 50.0, 0.0);

	return true;
}

bool Base::run(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	mRoot = new Ogre::Root(mPluginsCfg);
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

	mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);

	setupResources();

	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	mWindow = mRoot->initialise(true, "Base");
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	setupInput();
	createCamera();
	createScene();
	createOverlay();

	windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);
	mRoot->startRendering();

	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Base app;

		try {
			app.run();
		} catch(Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif