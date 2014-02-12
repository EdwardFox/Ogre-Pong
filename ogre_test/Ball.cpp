#include "Ball.h"


Ball::Ball(void) :
	mPos(0,0,0),
	mSpeed(0,0,0),
	mEntity(0),
	mEntityNode(0)
{
}


Ball::~Ball(void)
{
}

void Ball::create(Ogre::String id, Ogre::SceneManager* scMgr, Ogre::SceneNode* gameNode)
{
	mEntity = scMgr->createEntity(id, Ogre::SceneManager::PT_SPHERE);
	mEntity->setMaterialName("Examples/white");

	mEntityNode = gameNode->createChildSceneNode();
	mEntityNode->attachObject(mEntity);

	mEntityNode->setScale(0.20, 0.20, 0.20);
}

Ogre::Vector3 const Ball::getPosition() const
{
	return mPos;
}

Ogre::Vector3 const Ball::getSpeed() const
{
	return mSpeed;
}

void Ball::translate(const Ogre::Vector3 trans)
{
	mPos.x += trans.x;
	mPos.y += trans.y;
	mPos.z += trans.z;

	mEntityNode->translate(trans);
}

void Ball::translate(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz)
{
	this->translate(Ogre::Vector3(fx, fy, fz));
}

void Ball::setSpeed(const Ogre::Vector3 speed)
{
	mSpeed = speed;
}

void Ball::setSpeed(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz)
{
	this->setSpeed(Ogre::Vector3(fx, fy, fz));
}

void Ball::update(const Ogre::Real dt, const Bat& b1, const Bat& b2)
{
	Ogre::Real speedX = mSpeed.x*dt;
	Ogre::Real speedZ = mSpeed.z*dt;
	auto bounds = mEntity->getBoundingBox();

	auto ballMinX = mPos.x + bounds.getMinimum().x*mEntityNode->getScale().x + speedX;
	auto ballMaxX = mPos.x + bounds.getMaximum().x*mEntityNode->getScale().x + speedX;
	auto ballMinZ = mPos.z + bounds.getMinimum().z*mEntityNode->getScale().z + speedZ;
	auto ballMaxZ = mPos.z + bounds.getMaximum().z*mEntityNode->getScale().z + speedZ;

	// Sides
	if(ballMinX < -250 ||
		ballMaxX > 250)
	{
		auto sp = mSpeed;
		sp.x = -sp.x;
		mSpeed = sp;
	}

	// Bats
	if(this->collides(b1, dt) || this->collides(b2, dt)) {
		auto sp = mSpeed;
		sp.z = -sp.z;
		mSpeed = sp;
	}

	this->translate(mSpeed.x*dt, 0.0, mSpeed.z*dt);
}

bool Ball::collides(Bat b, Ogre::Real dt)
{
	Ogre::Real speedX = mSpeed.x*dt;
	Ogre::Real speedZ = mSpeed.z*dt;
	auto bounds = mEntity->getBoundingBox();

	auto ballMinX = mPos.x + bounds.getMinimum().x*mEntityNode->getScale().x + speedX;
	auto ballMaxX = mPos.x + bounds.getMaximum().x*mEntityNode->getScale().x + speedX;
	auto ballMinZ = mPos.z + bounds.getMinimum().z*mEntityNode->getScale().z + speedZ;
	auto ballMaxZ = mPos.z + bounds.getMaximum().z*mEntityNode->getScale().z + speedZ;

	bounds = b.getBoundingBox();
	auto bMinX = b.getPosition().x + bounds.getMinimum().x*b.getScale().x;
	auto bMaxX = b.getPosition().x + bounds.getMaximum().x*b.getScale().x;
	auto bMinZ = b.getPosition().z + bounds.getMinimum().z*b.getScale().z;
	auto bMaxZ = b.getPosition().z + bounds.getMaximum().z*b.getScale().z;

	if(
		((ballMinX > bMinX && ballMinX < bMaxX) || (ballMaxX > bMinX && ballMaxX < bMaxX)) &&
		((ballMinZ > bMinZ && ballMinZ < bMaxZ) || (ballMaxZ > bMinZ && ballMaxZ < bMaxZ)))
	{
		return true;
	}

	return false;
}

void Ball::setPosition(const Ogre::Vector3 pos)
{
	mPos = pos;
	mEntityNode->setPosition(pos);
}

void Ball::setPosition(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz)
{
	this->setPosition(Ogre::Vector3(fx, fy, fz));
}