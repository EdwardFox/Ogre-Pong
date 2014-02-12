#pragma once

#include <OgreRoot.h>
#include <OgreEntity.h>

#include "Bat.h"

class Ball
{
public:
	Ball(void);
	~Ball(void);

	void create(Ogre::String id, Ogre::SceneManager *scMgr);

	Ogre::Vector3 const getPosition() const;
	Ogre::Vector3 const getSpeed() const;

	void translate(const Ogre::Vector3 trans);
	void translate(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz);
	void setSpeed(const Ogre::Vector3 speed);
	void setSpeed(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz);
	void setPosition(const Ogre::Vector3 pos);
	void setPosition(const Ogre::Real fx, const Ogre::Real fy, const Ogre::Real fz);

	void update(const Ogre::Real dt, const Bat& b1, const Bat& b2);

private:
	bool collides(Bat b, Ogre::Real dt);

	Ogre::Vector3 mPos;
	Ogre::Vector3 mSpeed;

	Ogre::Entity *mEntity;
	Ogre::SceneNode *mEntityNode;
};

