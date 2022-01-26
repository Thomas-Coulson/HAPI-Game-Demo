#include "ExplosionEntity.h"
#include "World.h"

ExplosionEntity::ExplosionEntity(const std::string& currentSprite, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection) :
	Entity(currentSprite, posX, posY, alpha, clipped)
{
	m_currentDirection = currentDirection;
	m_entityType = EEntityType::eExplosion;
	m_explosionType = EExplosionType::eBullet;
}

ExplosionEntity::~ExplosionEntity() {}

void ExplosionEntity::update(World* world) {

	if (m_explosionType == EExplosionType::eBullet) {
		//if player spawns bullet - change to player bullet
		//if enemy spawns bullet - change to enemy bullet
		switch (m_currentDirection) {
		case EDirection::eLeft:
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireExplosionLeft";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicExplosionLeft";
			}
			break;
		case EDirection::eUp:
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireExplosionUp";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicExplosionUp";
			}
			break;
		case EDirection::eRight:
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireExplosionRight";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicExplosionRight";
			}
			break;
		case EDirection::eDown:
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireExplosionDown";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicExplosionDown";
			}
			break;
		default:
			break;
		}
	}
	else if (m_explosionType == EExplosionType::eEnemy) {
		m_currentSprite = "EnemyExplosion";
	}

	

	if (m_currentFrame >= world->getWindow()->getTextureMaxFrames(m_currentSprite) - 1) {
		//only play explosion animation once
		m_alive = false;
		m_moving = false;
	}

	if (m_alive) {
		m_lastPosition.setX(m_posX);
		m_lastPosition.setY(m_posY);
		m_nextPosition.setX(m_posX);
		m_nextPosition.setY(m_posY);
		m_moving = true;
	}
	else {
		m_currentFrame = 0;
		m_moving = false;
	}
	
}

void ExplosionEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	
}