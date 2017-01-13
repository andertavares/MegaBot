#include "AOEThreat.h"

AOEThreatClass::AOEThreatClass(Unit unit)
	: mUnit(unit)
	, mBullet(NULL)
{
}

AOEThreatClass::AOEThreatClass(BWAPI::Bullet bullet)
	: mUnit()
	, mBullet(bullet)
{
}

bool AOEThreatClass::hasGone() const
{
	return getPosition() == BWAPI::Positions::Invalid;
}

Position AOEThreatClass::getPosition() const
{
	if(mUnit && mUnit->exists())
	{
		const BWAPI::UnitType &type = mUnit->getType();
		if(type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		{
			if(mUnit->getTarget())
				return mUnit->getTarget()->getPosition();
			else
				return mUnit->getPosition();
		}
		else if(type == BWAPI::UnitTypes::Protoss_Scarab)
		{
			if(mUnit->getTarget())
				return mUnit->getTarget()->getPosition();
			else
				return BWAPI::Positions::Invalid;
		}
		else
			return mUnit->getPosition();
	}
	else if(mBullet && mBullet->exists())
	{
		const BWAPI::BulletType &type = mBullet->getType();
		if(type == BWAPI::BulletTypes::Psionic_Storm)
			return mBullet->getPosition();
		else if(type == BWAPI::BulletTypes::EMP_Missile)
			return mBullet->getTargetPosition();
	}

	return BWAPI::Positions::Invalid;
}

int AOEThreatClass::getRadius() const
{
	if(mUnit && mUnit->exists())
	{
		int radius = 0;
		if(isAirThreat())
			radius = mUnit->getType().airWeapon().medianSplashRadius();
		if(isGroundThreat())
			radius = std::max(mUnit->getType().groundWeapon().medianSplashRadius(), radius);
		
		if(!mUnit->getTarget())
			return radius / 2;
		else
			return radius;
	}
	else if(mBullet && mBullet->exists())
	{
		const BWAPI::BulletType &type = mBullet->getType();
		if(type == BWAPI::BulletTypes::Psionic_Storm)
			return 64;
		else if(type == BWAPI::BulletTypes::EMP_Missile)
			return 96;
	}

	return 0;
}

void AOEThreatClass::draw() const
{
	const Position &pos = getPosition();
	BWAPI::Broodwar->drawCircleMap(pos.x, pos.y, getRadius(), BWAPI::Colors::Red);
}

Unit AOEThreatClass::getTarget() const
{
	if(mUnit && mUnit->exists())
		return mUnit->getTarget();

	return StaticUnits::nullunit;
}

bool AOEThreatClass::isAirThreat() const
{
	if(mBullet && mBullet->exists())
	{
		if(mBullet->getType() == BWAPI::BulletTypes::Psionic_Storm)
			return true;
	}

	return false;
}

bool AOEThreatClass::isGroundThreat() const
{
	if(mUnit && mUnit->exists())
	{
		const BWAPI::UnitType &type = mUnit->getType();
		if(type == BWAPI::UnitTypes::Protoss_Scarab || type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
			return true;
	}
	else if(mBullet && mBullet->exists())
	{
		if(mBullet->getType() == BWAPI::BulletTypes::Psionic_Storm)
			return true;
	}

	return false;
}

bool AOEThreatClass::isEnergyThreat() const
{
	if(mBullet && mBullet->exists())
	{
		if(mBullet->getType() == BWAPI::BulletTypes::EMP_Missile)
			return true;
	}

	return false;
}