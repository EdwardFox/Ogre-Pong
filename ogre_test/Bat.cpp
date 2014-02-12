#include "Bat.h"


Bat::Bat(void) :
	mPos(0,0,0),
	mSpeed(0,0,0),
	mEntity(0),
	mEntityNode(0)
{
}


Bat::~Bat(void)
{
}

void Bat::create(Ogre::String id, Ogre::SceneManager* scMgr)
{
	mEntity = scMgr->createEntity(id, Ogre::SceneManager::PT_CUBE);
	mEntity->setMaterialName("Examples/white");

	mEntityNode = scMgr->getRootSceneNode()->createChildSceneNode();
	mEntityNode->attachObject(mEntity);

	mEntityNode->setScale(1.0, 0.25, 0.25);
}

Ogre::Vector3 const Bat::getPosition() const
{
	return mPos;
}

Ogre::Vector3 const Bat::getSpeed() const
{
	return mSpeed;
}

void Bat::translate(const Ogre::Vector3 trans)
{
	mPos.x += trans.x;
	mPos.y += trans.y;
	mPos.z += trans.z;

	mEntityNode->translate(trans);
}

void Bat::translate(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz)
{
	this->translate(Ogre::Vector3(fx, fy, fz));
}

void Bat::setSpeed(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz)
{
	this->setSpeed(Ogre::Vector3(fx, fy, fz));
}

void Bat::setSpeed(const Ogre::Vector3 speed)
{
	mSpeed = speed;
}

void Bat::update(Ogre::Real dt)
{
	Ogre::Real speed = mSpeed.x*dt;
	auto bounds = mEntity->getBoundingBox();

	// Position + width/2 + speed
	if(mPos.x + bounds.getMinimum().x + speed > -250 &&
		mPos.x + bounds.getMaximum().x + speed < 250)
	{
		this->translate(speed, 0.0, 0.0);
	}
}

Ogre::AxisAlignedBox const Bat::getBoundingBox() const
{
	return mEntity->getBoundingBox();
}

Ogre::Vector3 const Bat::getScale() const
{
	return mEntityNode->getScale();
}