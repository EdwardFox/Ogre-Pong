#pragma once

#include <OgreRoot.h>
#include <OgreEntity.h>

class Bat
{
public:
	Bat(void);
	~Bat(void);

	void create(Ogre::String id, Ogre::SceneManager *scMgr, Ogre::SceneNode* gameNode);

	Ogre::Vector3 const getPosition() const;
	Ogre::Vector3 const getSpeed() const;
	Ogre::AxisAlignedBox const getBoundingBox() const;
	Ogre::Vector3 const getScale() const;

	void translate(const Ogre::Vector3 trans);
	void translate(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz);
	void setSpeed(const Ogre::Vector3 speed);
	void setSpeed(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz);

	void update(Ogre::Real dt);

private:
	void followBall();

	Ogre::Vector3 mPos;
	Ogre::Vector3 mSpeed;

	Ogre::Entity *mEntity;
	Ogre::SceneNode *mEntityNode;
};

